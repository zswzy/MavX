/*****************************************************************//**
 * @file   mavx_callbackfunctions.h
 * @brief  Declaration of callback functions
 * 
 * @author zswzy
 * @date   12/4/2024
 *********************************************************************/

#ifndef MAVX_CALLBACKFUNCTIONS_H
#define MAVX_CALLBACKFUNCTIONS_H

 /**
  * @brief Main callback function. This function is executed periodically. Each time it
  * collect flight data, and send to the destination (could be the QGroundControl) via
  * Mavlink message protocol.
  *
  * @param inElapsedSinceLastCall
  * @param inElapsedTimeSinceLastFlightLoop
  * @param inCounter
  * @param inRefcon
  * @return float£¬XP will execute this function after no early then this value in second.
  */
float CallBackDataReadAndSend(float elapsedMe, float elapsedSim, int counter, void* refcon);

#endif // !MAVX_CALLBACKFUNCTIONS_H

