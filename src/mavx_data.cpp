#include "mavx_data.h"

int FindDataRef(XpRef& xpref)
{
    xpref.ref_total_flight_time_sec = XPLMFindDataRef("sim/time/total_flight_time_sec");
    xpref.ref_longitude             = XPLMFindDataRef("sim/flightmodel/position/longitude");
    xpref.ref_latitude              = XPLMFindDataRef("sim/flightmodel/position/latitude");
    xpref.ref_elevation             = XPLMFindDataRef("sim/flightmodel/position/elevation");
    xpref.ref_true_psi              = XPLMFindDataRef("sim/flightmodel/position/true_psi");
    xpref.ref_y_agl                 = XPLMFindDataRef("sim/flightmodel/position/y_agl");

    return 0;
}


int GetDataRef(XpRef& xpref, XpData& xpdata)
{
    xpdata.total_flight_time_sec    = XPLMGetDataf(xpref.ref_total_flight_time_sec);
    xpdata.longitude                = XPLMGetDataf(xpref.ref_longitude);
    xpdata.latitude                 = XPLMGetDataf(xpref.ref_latitude);
    xpdata.elevation                = XPLMGetDataf(xpref.ref_elevation);
    xpdata.true_psi                 = XPLMGetDataf(xpref.ref_true_psi);
    xpdata.y_agl                    = XPLMGetDataf(xpref.ref_y_agl);

    return 0;
}
