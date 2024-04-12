/*****************************************************************//**
 * @file   mavx_data.h
 * @brief  Data structures here.
 * 
 * @author zeyuan
 * @date   12/4/2024
 *********************************************************************/

#ifndef MAVX_DATA_H
#define MAVX_DATA_H

#include "XPLMPlugin.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

/**
 * @brief Save XP reference data (pointer).
 */
typedef struct  _XpRef {
	XPLMDataRef ref_total_flight_time_sec	= NULL;
	XPLMDataRef ref_longitude				= NULL;
	XPLMDataRef ref_latitude				= NULL;
	XPLMDataRef ref_elevation				= NULL;  // meters
	XPLMDataRef ref_true_psi				= NULL; // The heading of the aircraft relative to the earth precisely below the aircraft - true degrees north, always
	XPLMDataRef ref_y_agl					= NULL; // AGL meters
} XpRef;


/**
 * @brief Save XP raw data (values).
 */
typedef struct _XpData {
	double total_flight_time_sec	= 0.0;
	double longitude				= 0.0;
	double latitude					= 0.0;
	double elevation				= 0.0;
	double true_psi					= 0.0; // The heading of the aircraft relative to the earth precisely below the aircraft - true degrees north, always
	double y_agl					= 0.0; // AGL meters
} XpData;





int FindDataRef(XpRef &xpref);


int GetDataRef(XpRef &xpref, XpData &xpdata);


#endif // !MAV_DATA_H