/*****************************************************************//**
 * @file   mavx_xplmain.cpp
 * @brief  Export Dynamic link library .xpl file
 * @details This plugin allows you to monitoring the aircraft's stats
 * from QGroundControl.
 * 
 * @author zswzy
 * @date   12/4/2024
 *********************************************************************/

#include "mavx_framework.h"




//======================================================================================
// Global variables
// Log file
static FILE* g_output_file;

// XP ref data
XpRef xpref;

// XP raw data value
XpData xpdata;

// Mavlink
// Instantiate Mavsdk
auto component_type = mavsdk::Mavsdk::ComponentType::Autopilot;
auto config = mavsdk::Mavsdk::Configuration(component_type);
auto mav = mavsdk::Mavsdk(config);

// UDP
auto udp_qgc = UdpCom();  //!< UDP for QGroundControl
//======================================================================================


float interval_read_and_send = 0.005;

//======================================================================================

/**
 * @brief Plugin Init
 *
 * @param outName   plugin name
 * @param outSig    plugin signature
 * @param outDesc   plugin description
 * @return 1: normal
 */
PLUGIN_API int XPluginStart(
	char* outName,
	char* outSig,
	char* outDesc)
{

	// plugin info
	strcpy(outName, "MavX");
	strcpy(outSig, "MavX.plugin");
	strcpy(outDesc, "Read flight data and communicate with QGroundControl via Mavlink");
	// Open Log file
	char	outputPath[255];
	XPLMGetSystemPath(outputPath);
	strcat(outputPath, "LogMavX.txt");
	g_output_file = fopen(outputPath, "w");
	fprintf(g_output_file, "MavX plugin starts.\n");

	// Link data ref
	FindDataRef(xpref);
	

	// Register call back function 
	XPLMRegisterFlightLoopCallback(CallBackDataReadAndSend, interval_read_and_send, NULL);

	// Open UDP
	int r_udp_open = udp_qgc.Open("127.0.0.1", 14540, "127.0.0.1", 14550);

	return 1;
}

//======================================================================================

/**
 * @brief Execute while plugin is stopped
 *
 * @return void
 */
PLUGIN_API void XPluginStop(void)
{
	// Log
	fprintf(g_output_file, "MavX plugin is stopped.\n");

	// Unregister call back function
	XPLMUnregisterFlightLoopCallback(CallBackDataReadAndSend, NULL);

	// Close file
	fclose(g_output_file);

	// Close UDP
	udp_qgc.Close();
}

//======================================================================================

/**
 * @brief Execute while the plugin is disabled
 *
 * @return void
 */
PLUGIN_API void XPluginDisable(void)
{
	// Log
	fprintf(g_output_file, "MavX plugin is disabled.\n");

	// flush, useful to check log file when plugin disabled
	fflush(g_output_file);
}

//======================================================================================

/**
 * @brief Execute while the plugin is enabled
 *
 * @return 1 normal
 */
PLUGIN_API int XPluginEnable(void)
{
	// Log
	fprintf(g_output_file, "MavX plugin is enabled.\n");

	return 1;
}

//======================================================================================


float CallBackDataReadAndSend(
	float       inElapsedSinceLastCall,
	float       inElapsedTimeSinceLastFlightLoop,
	int         inCounter,
	void* inRefcon)
{
	// Get data values
	GetDataRef(xpref, xpdata);

	// Save to Log
	//fprintf(g_output_file, "true pitch: %.4f deg, flight time: %.4f s.\n", true_theta_deg, total_flight_time_sec);

	// Prepare Mavlink Msg
	// heartbeats
	mavlink_message_t msg_heartbeat;
	uint8_t len_heartbeat;
	uint8_t buf_heartbeat[MAVLINK_MAX_PACKET_LEN];
	mavlink_msg_heartbeat_pack(config.get_system_id(), config.get_component_id(), &msg_heartbeat,
		MAV_TYPE_GENERIC, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
	len_heartbeat = mavlink_msg_to_send_buffer(buf_heartbeat, &msg_heartbeat);

	// position
	mavlink_message_t msg_position;
	uint8_t len_position;
	uint8_t buf_position[MAVLINK_MAX_PACKET_LEN];
	int32_t lat = xpdata.latitude * 1E7;
	int32_t lon = xpdata.longitude * 1E7;
	int32_t alt = xpdata.elevation * 1E3;
	int32_t relative_alt = xpdata.y_agl * 1E3;
	uint16_t hdg = xpdata.true_psi;
	mavlink_msg_global_position_int_pack(config.get_system_id(), config.get_component_id(), &msg_position,
		xpdata.total_flight_time_sec * 1000, lat, lon, alt, relative_alt, 0, 0, 0, hdg);
	len_position = mavlink_msg_to_send_buffer(buf_position, &msg_position);

	// Send Message
	udp_qgc.Send((char*)(buf_heartbeat), len_heartbeat);
	udp_qgc.Send((char*)(buf_position), len_position);

	return interval_read_and_send;
}


