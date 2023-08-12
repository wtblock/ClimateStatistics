/////////////////////////////////////////////////////////////////////////////
// Copyright © 2023 by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "atlcomtime.h"
#include "KeyedCollection.h"
#include "CHelper.h"

/////////////////////////////////////////////////////////////////////////////
// a class to store yearly statistics
class CYearlyStatistics
{
// public definitions
public:

// protected data
protected:
	// collection of stations read for the year
	// and the count of valid readings for each
	// station
	CKeyedCollection<CString,int> m_Stations;
	// date of the maximum temperature measurement
	CString m_csDate;
	// state of the maximum temperature measurement
	CString m_csState;
	// station of the maximum temperature measurement
	CString m_csStation;
	// maximum temperature for the year
	float m_fMaximumTemperature;
	// average high temperature
	float m_fAverageTemperature;
	// number of valid readings for the year
	int m_nReadings;
	// number of valid stations for the year
	int m_nStations;
	// number of days over 90
	int m_nOver90;
	// number of days over 95
	int m_nOver95;
	// number of days over 100
	int m_nOver100;
	// number of days over 105
	int m_nOver105;
	// number of days over 110
	int m_nOver110;
	// number of days over 115
	int m_nOver115;
	// number of days over 120
	int m_nOver120;
	// number of days over 125
	int m_nOver125;
	// number of days over 130
	int m_nOver130;

// public properties
public:
	// test the validity of the value for the given station
	// and if valid update the count for the station and
	// keep track of the stations that supplied valid data
	inline bool GetValid( float fValue, CString csStation )
	{
		bool value = true;
		const bool bMissing = 
			CHelper::NearlyEqual( fValue, -9999.0f );
		const bool bTooHigh = fValue > 140.0f;
		if ( bMissing || bTooHigh )
		{
			value = false;
		}
		else
		{
			shared_ptr<int> pCount;
			if ( m_Stations.Exists[ csStation ] )
			{
				pCount = m_Stations.find( csStation );
			}
			else
			{
				pCount = shared_ptr<int>( new int( 0 ) );
				m_Stations.add( csStation, pCount );
			}

			// update number of valid readings for this station
			(*pCount)++;

			// calculate the new average temperature
			const float fTotal = 
				m_fAverageTemperature * m_nReadings + fValue;
			m_nReadings++;
			m_fAverageTemperature = fTotal / m_nReadings;
		}

		return value;
	}
	// test the validity of the value for the given station
	// and if valid update the count for the station and
	// keep track of the stations that supplied valid data
	__declspec( property( get = GetValid ) )
		bool Valid[][];

	// CSV Header
	inline CString GetHeader()
	{
		CString value = 
			_T( "YEAR,DATE,STATE,STATION,MAXDEGF,AVGDEGF,READINGS,STATIONS," )
			_T( ">90,>95,>100,>105,>110,>115,>120,>125,>130\n" );

		return value;
	}
	// CSV Header
	__declspec( property( get = GetHeader ) )
		CString Header;

	// CSV Header
	inline CString GetData( CString csYear )
	{
		CString value;
		value.Format
		(
			_T( "%s,%s,%s,%s,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n" ),
			csYear, Date, State, Station, 
			MaximumTemperature, AverageTemperature,
			Readings, Stations, Over90, Over95, Over100, Over105, 
			Over110, Over115, Over120, Over125, Over130
		);

		return value;
	}
	// CSV data line
	__declspec( property( get = GetData ) )
		CString Data[];

	// date of the maximum temperature measurement
	inline CString GetDate()
	{
		return m_csDate;
	}
	// date of the maximum temperature measurement
	inline void SetDate( CString value )
	{
		m_csDate = value;
	}
	// date of the maximum temperature measurement
	__declspec( property( get = GetDate, put = SetDate ) )
		CString Date;

	// state of the maximum temperature measurement
	inline CString GetState()
	{
		return m_csState;
	}
	// state of the maximum temperature measurement
	inline void SetState( CString value )
	{
		m_csState = value;
	}
	// state of the maximum temperature measurement
	__declspec( property( get = GetState, put = SetState ) )
		CString State;

	// station of the maximum temperature measurement
	inline CString GetStation()
	{
		return m_csStation;
	}
	// station of the maximum temperature measurement
	inline void SetStation( CString value )
	{
		m_csStation = value;
	}
	// station of the maximum temperature measurement
	__declspec( property( get = GetStation, put = SetStation ) )
		CString Station;

	// maximum temperature for the year
	inline float GetMaximumTemperature()
	{
		return m_fMaximumTemperature;
	}
	// maximum temperature for the year
	inline void SetMaximumTemperature( float value )
	{
		m_fMaximumTemperature = value;
	}
	// maximum temperature for the year
	__declspec
	( 
		property
		( 
			get = GetMaximumTemperature, 
			put = SetMaximumTemperature 
		) 
	)
	float MaximumTemperature;

	// maximum temperature for the year
	inline float GetAverageTemperature()
	{
		return m_fAverageTemperature;
	}
	// maximum temperature for the year
	inline void SetAverageTemperature( float value )
	{
		m_fAverageTemperature = value;
	}
	// maximum temperature for the year
	__declspec
	( 
		property
		( 
			get = GetAverageTemperature, 
			put = SetAverageTemperature 
		) 
	)
	float AverageTemperature;

	// number of days over 90
	inline int GetOver90()
	{
		return m_nOver90;
	}
	// number of days over 90
	inline void SetOver90( int value )
	{
		m_nOver90 = value;
	}
	// number of days over 90
	__declspec( property( get = GetOver90, put = SetOver90 ) )
		int Over90;

	// number of valid readings for the year
	inline int GetReadings()
	{
		return m_nReadings;
	}
	// number of valid readings for the year
	inline void SetReadings( int value )
	{
		m_nReadings = value;
	}
	// number of valid readings for the year
	__declspec( property( get = GetReadings, put = SetReadings ) )
		int Readings;

	// number of valid stations for the year
	inline int GetStations()
	{
		int value = (int)m_Stations.Count;
		Stations = value;
		return value;
	}
	// number of valid stations for the year
	inline void SetStations( int value )
	{
		m_nStations = value;
	}
	// number of valid stations for the year
	__declspec( property( get = GetStations, put = SetStations ) )
		int Stations;

	// number of days over 95
	inline int GetOver95()
	{
		return m_nOver95;
	}
	// number of days over 95
	inline void SetOver95( int value )
	{
		m_nOver95 = value;
	}
	// number of days over 95
	__declspec( property( get = GetOver95, put = SetOver95 ) )
		int Over95;

	// number of days over 100
	inline int GetOver100()
	{
		return m_nOver100;
	}
	// number of days over 100
	inline void SetOver100( int value )
	{
		m_nOver100 = value;
	}
	// number of days over 100
	__declspec( property( get = GetOver100, put = SetOver100 ) )
		int Over100;

	// number of days over 105
	inline int GetOver105()
	{
		return m_nOver105;
	}
	// number of days over 105
	inline void SetOver105( int value )
	{
		m_nOver105 = value;
	}
	// number of days over 105
	__declspec( property( get = GetOver105, put = SetOver105 ) )
		int Over105;

	// number of days over 110
	inline int GetOver110()
	{
		return m_nOver110;
	}
	// number of days over 110
	inline void SetOver110( int value )
	{
		m_nOver110 = value;
	}
	// number of days over 110
	__declspec( property( get = GetOver110, put = SetOver110 ) )
		int Over110;

	// number of days over 115
	inline int GetOver115()
	{
		return m_nOver115;
	}
	// number of days over 115
	inline void SetOver115( int value )
	{
		m_nOver115 = value;
	}
	// number of days over 115
	__declspec( property( get = GetOver115, put = SetOver115 ) )
		int Over115;

	// number of days over 120
	inline int GetOver120()
	{
		return m_nOver120;
	}
	// number of days over 120
	inline void SetOver120( int value )
	{
		m_nOver120 = value;
	}
	// number of days over 120
	__declspec( property( get = GetOver120, put = SetOver120 ) )
		int Over120;

	// number of days over 125
	inline int GetOver125()
	{
		return m_nOver125;
	}
	// number of days over 125
	inline void SetOver125( int value )
	{
		m_nOver125 = value;
	}
	// number of days over 125
	__declspec( property( get = GetOver125, put = SetOver125 ) )
		int Over125;

	// number of days over 130
	inline int GetOver130()
	{
		return m_nOver130;
	}
	// number of days over 130
	inline void SetOver130( int value )
	{
		m_nOver130 = value;
	}
	// number of days over 130
	__declspec( property( get = GetOver130, put = SetOver130 ) )
		int Over130;

// protected methods
protected:

// public methods
public:
	// test the given value against the over values
	// and increase the associated counts if necessary
	void UpdateOverValues( float value )
	{
		if ( value > 90.0f )
		{
			m_nOver90++;
			if ( value > 95.0f )
			{
				m_nOver95++;
				if ( value > 100.0f )
				{
					m_nOver100++;
					if ( value > 105.0f )
					{
						m_nOver105++;
						if ( value > 110.0f )
						{
							m_nOver110++;
							if ( value > 115.0f )
							{
								m_nOver115++;
								if ( value > 120.0f )
								{
									m_nOver120++;
									if ( value > 125.0f )
									{
										m_nOver125++;
										if ( value > 130.0f )
										{
											m_nOver130++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
// protected overrides
protected:

// public overrides
public:

// public construction / destruction
public:
	CYearlyStatistics()
	{
		Readings = 0;
		Stations = 0;
		MaximumTemperature = -9999.0f;
		AverageTemperature = -9999.0f;
		Over90 = 0;
		Over95 = 0;
		Over100 = 0;
		Over105 = 0;
		Over110 = 0;
		Over115 = 0;
		Over120 = 0;
		Over125 = 0;
		Over130 = 0;
	}
	virtual ~CYearlyStatistics()
	{

	}
};

