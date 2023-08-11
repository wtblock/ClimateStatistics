/////////////////////////////////////////////////////////////////////////////
// Copyright © 2023 by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ClimateStatistics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object
CWinApp theApp;

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// parse the state name from the current path
bool ParseState()
{
	bool value = true;
	// parse the station code and state name from the folder name
	const CString csFolder =
		CHelper::GetDirectory( m_csPath ).TrimRight( _T( "\\" ) );
	m_csState = CHelper::GetFileName( csFolder );
	const int nLen = m_csState.GetLength();
	if ( nLen != 2 )
	{
		value = false;
	}

	return value;
} // ParseState

/////////////////////////////////////////////////////////////////////////////
// write the statistics to the output file
void WriteOutput( CStdioFile& fErr )
{
	// write the data into a file named after the two character
	// state abbreviation with a CSV extension
	CString csFolder = CHelper::GetDirectory( m_csPath );
	csFolder.TrimRight( _T( "\\" ));
	CString csData;
	csData.Format( _T( "%s\\ClimateStatistics.CSV" ), csFolder );

	fErr.WriteString( _T( "Writing output: \n\t" + csData + _T( "\n" )));

	// create the file if it does not exist or replace the data
	// if the file does exist
	CStdioFile fileWrite;
	const bool value =
		FALSE != fileWrite.Open
		(
			csData, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone
		);

	bool bFirst = true;
	if ( value == true )
	{
		for ( auto& node : m_Statistics.Items )
		{
			const CString csYear = node.first;
			fErr.WriteString( _T( "Year: " ) + csYear + _T( "\n" ));

			if ( bFirst )
			{
				bFirst = false;
				const CString csHeader = node.second->Header; 
				fileWrite.WriteString( csHeader );
			}
			
			const CString csLine = node.second->Data[ csYear ];
			fileWrite.WriteString( csLine );
		}

		fileWrite.Close();
	}

} // WriteOutput

/////////////////////////////////////////////////////////////////////////////
// process the current state 
void ProcessState( CStdioFile& fErr )
{
	// open the stations text file
	CStdioFile file;
	const bool value =
		FALSE != file.Open
		(
			m_csPath, CFile::modeRead | CFile::shareDenyNone
		);

	// if the open was successful, read each line of the file and 
	// collect the data properties
	if ( value == true )
	{
		bool bFirst = true;
		CString csHeader;
		CString csLine;
		const CString csDelim( _T( "," ) );
		while ( file.ReadString( csLine ) )
		{
			// the first token of the line is the date: yyyymmdd
			int nStartLine = 0;
			int nStartHead = 0;
			CString csHead;
			const CString csDate = csLine.Tokenize( csDelim, nStartLine );

			// record the CSV header line
			if ( csDate == _T( "Date" ) )
			{
				csHeader = csLine;
				csHead = csHeader.Tokenize( csDelim, nStartHead );
				continue;
			}

			// year of this statistic
			CString csYear = csDate.Left( 4 );

			shared_ptr<CYearlyStatistics> pStats;

			// find or create a statistic date
			if ( m_Statistics.Exists[ csYear ] )
			{
				pStats = m_Statistics.find( csYear );
			}
			else
			{
				pStats = shared_ptr<CYearlyStatistics>( new CYearlyStatistics );
				m_Statistics.add( csYear, pStats );
			}

			// the current maximum for this year
			float fMax = pStats->MaximumTemperature;
			CString csStationMax = pStats->Station;
			CString csDateMax = pStats->Date;
			bool bMax = false;
			do
			{
				const CString csToken = csLine.Tokenize( csDelim, nStartLine );
				m_csStation = csHeader.Tokenize( csDelim, nStartHead );
				if ( csToken.IsEmpty() )
				{
					break;
				}
				const float fTemp = (float)_tstof( csToken );
				if ( fTemp > fMax )
				{
					fMax = fTemp;
					csStationMax = m_csStation;
					csDateMax = csDate;
					bMax = true;
				}
			}
			while ( true );
			if ( bMax )
			{
				pStats->UpdateOverValues( fMax );
				pStats->MaximumTemperature = fMax;
				pStats->Date = csDateMax;
				pStats->Station = csStationMax;
				pStats->State = m_csState;
			}
		}
	}
} // ProcessState

/////////////////////////////////////////////////////////////////////////////
// crawl through the directory tree looking for wild card files
void RecursePath( CStdioFile& fErr, LPCTSTR path )
{
	USES_CONVERSION;

	// start trolling for files we are interested in
	CFileFind finder;
	BOOL bWorking = finder.FindFile( path );
	while ( bWorking )
	{
		bWorking = finder.FindNextFile();

		// skip "." and ".." folder names
		if ( finder.IsDots() )
		{
			continue;
		}

		// if it's a directory, recursively search it
		if ( finder.IsDirectory() )
		{
			const CString str = finder.GetFilePath();
			m_csFilename = CHelper::GetFileName( str );

			// if the state changes, process previous state
			if ( m_csFilename != m_csState )
			{
				// write a message to the console update the status
				CString csMessage;
				csMessage.Format
				( 
					_T( "State changed from %s to %s\n" ), 
					m_csState, m_csFilename 
				);
				fErr.WriteString( csMessage );

				// update to the new state
				m_csState = m_csFilename;
				m_csWildcard.Format( _T( "\\%s.CSV" ), m_csState );
			}

			// handle this directory data
			RecursePath( fErr, str + m_csWildcard );
		}
		else // process if it is a valid filename
		{
			m_csPath = finder.GetFilePath().MakeUpper();
			m_csFilename = CHelper::GetFileName( m_csPath );
			bool bOkay = ParseState();
			const CString csExt = CHelper::GetExtension( m_csPath );
			if ( bOkay && csExt == _T( ".CSV" ) && m_csFilename == m_csState )
			{
				// write a status message to the console
				CString csMessage;
				csMessage.Format( _T( "%s\n" ), m_csPath );
				fErr.WriteString( csMessage );

				// process the station data
				ProcessState( fErr );
			}
		}
	}

	// done with the finder
	finder.Close();

} // RecursePath

/////////////////////////////////////////////////////////////////////////////
// read the stations text file and index it by the station 6 digit station 
// code
bool ReadStations()
{
	// open the stations text file
	CStdioFile file;
	const bool value =
		FALSE != file.Open
		(
			m_csStationPath, CFile::modeRead | CFile::shareDenyNone
		);

	// if the open was successful, read each line of the file and 
	// collect the station data properties
	if ( value == true )
	{
		CString csLine;
		while ( file.ReadString( csLine ) )
		{
			shared_ptr<CClimateStation> pStation =
				shared_ptr<CClimateStation>( new CClimateStation( csLine ) );
			const CString csKey = pStation->Station;
			m_Stations.add( csKey, pStation );
		}
	}

	return value;
} // ReadStations

  /////////////////////////////////////////////////////////////////////////////
  // read the state data and make a cross reference between the two digit code
  // and the two letter postal code that is used to identify states
bool ReadStates()
{
	bool value = false;

	// create an array of tuples containing the state code, abbreviation, 
	// and name
	const CString states[] =
	{
		_T( "01,AL,Alabama" ),
		_T( "02,AZ,Arizona" ),
		_T( "03,AR,Arkansas" ),
		_T( "04,CA,California" ),
		_T( "05,CO,Colorado" ),
		_T( "06,CT,Connecticut" ),
		_T( "07,DE,Delaware" ),
		_T( "08,FL,Florida" ),
		_T( "09,GA,Georgia" ),
		_T( "10,ID,Idaho" ),
		_T( "11,IL,Illinois" ),
		_T( "12,IN,Indiana" ),
		_T( "13,IA,Iowa" ),
		_T( "14,KS,Kansas" ),
		_T( "15,KY,Kentucky" ),
		_T( "16,LA,Louisiana" ),
		_T( "17,ME,Maine" ),
		_T( "18,MD,Maryland" ),
		_T( "19,MA,Massachusetts" ),
		_T( "20,MI,Michigan" ),
		_T( "21,MN,Minnesota" ),
		_T( "22,MS,Mississippi" ),
		_T( "23,MO,Missouri" ),
		_T( "24,MT,Montana" ),
		_T( "25,NE,Nebraska" ),
		_T( "26,NV,Nevada" ),
		_T( "27,NH,New Hampshire" ),
		_T( "28,NJ,New Jersey" ),
		_T( "29,NM,New Mexico" ),
		_T( "30,NY,New York" ),
		_T( "31,NC,North Carolina" ),
		_T( "32,ND,North Dakota" ),
		_T( "33,OH,Ohio" ),
		_T( "34,OK,Oklahoma" ),
		_T( "35,OR,Oregon" ),
		_T( "36,PA,Pennsylvania" ),
		_T( "37,RI,Rhode Island" ),
		_T( "38,SC,South Carolina" ),
		_T( "39,SD,South Dakota" ),
		_T( "40,TN,Tennessee" ),
		_T( "41,TX,Texas" ),
		_T( "42,UT,Utah" ),
		_T( "43,VT,Vermont" ),
		_T( "44,VA,Virginia" ),
		_T( "45,WA,Washington" ),
		_T( "46,WV,West Virginia" ),
		_T( "47,WI,Wisconsin" ),
		_T( "48,WY,Wyoming" )
	};

	// build a cross reference to this information
	for ( auto& node : states )
	{
		int nStart = 0;
		CString csToken;
		const CString csDelim = _T( "," );
		do
		{
			const CString csCode = node.Tokenize( csDelim, nStart );
			if ( csCode.IsEmpty() )
			{
				break;
			}
			const CString csPostal = node.Tokenize( csDelim, nStart );
			const CString csName = node.Tokenize( csDelim, nStart );
			shared_ptr<CState> pState( new CState );
			pState->Code = csCode;
			pState->Postal = csPostal;
			pState->Name = csName;

			// index by two letter state abbreviation
			m_States.add( csPostal, pState );
			shared_ptr<CString> pPostal( new CString( csPostal ) );

			// build a cross reference indexed by the numerical code
			m_StateCodes.add( csCode, pPostal );
		}
		while ( true );

		value = true;
	}

	return value;
} // ReadStates

  /////////////////////////////////////////////////////////////////////////////
  // a simple command line application to illustrate a bug in command line
  // processing
int _tmain( int argc, TCHAR* argv[], TCHAR* envp[] )
{
	HMODULE hModule = ::GetModuleHandle( NULL );
	if ( hModule == NULL )
	{
		_tprintf( _T( "Fatal Error: GetModuleHandle failed\n" ) );
		return 1;
	}

	// initialize MFC and error on failure
	if ( !AfxWinInit( hModule, NULL, ::GetCommandLine(), 0 ) )
	{
		_tprintf( _T( "Fatal Error: MFC initialization failed\n " ) );
		return 2;
	}

	// do some common command line argument corrections
	vector<CString> arrArgs = CHelper::CorrectedCommandLine( argc, argv );
	size_t nArgs = arrArgs.size();

	CStdioFile fErr( stderr );
	CString csMessage;

	// display the number of arguments if not 1 to help the user 
	// understand what went wrong if there is an error in the
	// command line syntax
	if ( nArgs != 1 )
	{
		fErr.WriteString( _T( ".\n" ) );
		csMessage.Format
		(
			_T( "The number of parameters are %d\n.\n" ), nArgs - 1
		);
		fErr.WriteString( csMessage );

		// display the arguments
		for ( size_t i = 1; i < nArgs; i++ )
		{
			csMessage.Format
			(
				_T( "Parameter %d is %s\n.\n" ), i, arrArgs[ i ]
			);
			fErr.WriteString( csMessage );
		}
	}

	// two arguments if a pathname to the climate data is given
	// three arguments if the station text file name is also given
	if ( nArgs != 2 && nArgs != 3 )
	{
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString
		(
			_T( "ClimateStatistics, Copyright (c) 2023, " )
			_T( "by W. T. Block.\n" )
		);

		fErr.WriteString
		(
			_T( ".\n" )
			_T( "A Windows command line program to read daily state \n" )
			_T( "  climate history from state folders containing .\n" )
			_T( "  SS.CSV files where SS is the two letter state .\n" )
			_T( "  abbreviation. This data is organized by date \n" )
			_T( "  where each column is a maximum temperature reading \n" )
			_T( "  from each station in the state. The output from \n" )
			_T( "  this program are statistics for the whole country \n" )
			_T( "  organized on a yearly basis in a single CSV file.\n" )
			_T( ".\n" )
		);

		fErr.WriteString
		(
			_T( ".\n" )
			_T( "Usage:\n" )
			_T( ".\n" )
			_T( ".  ClimateStatistics pathname [station_file_name]\n" )
			_T( ".\n" )
			_T( "Where:\n" )
			_T( ".\n" )
		);

		fErr.WriteString
		(
			_T( ".  pathname is the folder to be scanned that contains\n" )
			_T( ".    all of the state folders that contain SS.CSV files \n" )
			_T( ".    where, SS  is the two letter state abbreviation code. \n" )
			_T( ".  station_file_name is the optional station filename: \n" )
			_T( ".    defaults to: \"stations.txt\"\n" )
			_T( ".\n" )
		);

		return 3;
	}

	// display the executable path
	csMessage.Format( _T( "Executable pathname: %s\n" ), arrArgs[ 0 ] );
	fErr.WriteString( _T( ".\n" ) );
	fErr.WriteString( csMessage );
	fErr.WriteString( _T( ".\n" ) );

	// retrieve the pathname from the command line
	m_csPath = arrArgs[ 1 ];

	// trim off any wild card data
	const CString csFolder = CHelper::GetFolder( m_csPath );

	// test for current folder character (a period)
	bool bExists = m_csPath == _T( "." );

	// if it is a period, add a wild card of *.* to retrieve
	// all folders and files
	if ( !bExists )
	{
		if ( ::PathFileExists( csFolder ) )
		{
			bExists = true;
		}
	}

	if ( !bExists )
	{
		csMessage.Format( _T( "Invalid pathname:\n\t%s\n" ), m_csPath );
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
		fErr.WriteString( _T( ".\n" ) );
		return 4;

	}
	else
	{
		csMessage.Format( _T( "Given pathname:\n\t%s\n" ), m_csPath );
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
	}

	// read the station data
	m_csStationPath = _T( "stations.txt" );
	if ( nArgs == 3 )
	{
		m_csStationPath = arrArgs[ 2 ];
	}

	if ( !::PathFileExists( m_csStationPath ) )
	{
		csMessage.Format
		(
			_T( "Invalid Station path name:\n\t%s\n" ), m_csStationPath
		);
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
		fErr.WriteString( _T( ".\n" ) );
		return 5;

	}

	// start up COM
	AfxOleInit();
	::CoInitialize( NULL );

	// the pathname of the executable
	const CString csExe = arrArgs[ 0 ];

	// create a context block so the project goes 
	// out of context before the program ends to prevent
	// memory leaks
	{
		// read the state metadata
		ReadStates();

		// read the station metadata
		ReadStations();

		// build a path containing wild card data
		const CString csRight = m_csPath.Right( 1 );
		if ( csRight != _T( "\\" ) )
		{
			m_csPath += _T( "\\" );
		}
		m_csPath += _T( "*.*" );

		// process the entire directory tree from the first parameter
		RecursePath( fErr, m_csPath );

		// generate the output file
		WriteOutput( fErr );
	}

	// all is good
	return 0;

} // _tmain

  /////////////////////////////////////////////////////////////////////////////
