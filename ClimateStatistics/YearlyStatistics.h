/////////////////////////////////////////////////////////////////////////////
// Copyright © 2023 by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "atlcomtime.h"

/////////////////////////////////////////////////////////////////////////////
// a class to store yearly statistics
class CYearlyStatistics
{
// public definitions
public:

// protected data
protected:
	// date of the maximum temperature measurement
	CString m_csDate;
	// state of the maximum temperature measurement
	CString m_csState;
	// station of the maximum temperature measurement
	CString m_csStation;
	// maximum temperature for the year
	float m_fMaximumTemperature;
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
	// CSV Header
	inline CString GetHeader()
	{
		CString value = 
			_T( "YEAR,DATE,STATE,STATION,DEGF," )
			_T( ">90,>95,>100,>105,>110,>115," )
			_T( ">120,>125,>130\n" );

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
			_T( "%s,%s,%s,%s,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d\n" ),
			csYear, Date, State, Station, MaximumTemperature,
			Over90, Over95, Over100, Over105, Over110,
			Over115, Over120, Over125, Over130
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
		MaximumTemperature = -9999.0f;
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

