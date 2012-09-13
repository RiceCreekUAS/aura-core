// route_mgr.hxx - manage a route (i.e. a collection of waypoints)
//
// Written by Curtis Olson, started January 2004.
//
// Copyright (C) 2004  Curtis L. Olson  - http://www.flightgear.org/~curt
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// $Id: route_mgr.hxx,v 1.11 2008/11/23 04:02:17 curt Exp $


#ifndef _ROUTE_MGR_HXX
#define _ROUTE_MGR_HXX

#ifndef __cplusplus
# error This library requires C++
#endif

#include <string>
#include <vector>

using std::string;
using std::vector;

#include "props/props.hxx"
#include "route.hxx"


/**
 * Top level route manager class
 * 
 */

class FGRouteMgr {

private:

    SGRoute *route;

    // route configuration tree
    SGPropertyNode *config_props;

    // automatic inputs
    SGPropertyNode *lon_node;
    SGPropertyNode *lat_node;
    SGPropertyNode *alt_node;

    // automatic outputs
    SGPropertyNode *true_hdg_deg;
    SGPropertyNode *target_agl_ft;
    SGPropertyNode *override_agl_ft;
    SGPropertyNode *target_msl_ft;
    SGPropertyNode *override_msl_ft;
    SGPropertyNode *target_waypoint;
    SGPropertyNode *wp_dist_m;
    SGPropertyNode *wp_eta_sec;

    // wind related property nodes
    SGPropertyNode *wind_speed_kt;
    SGPropertyNode *wind_dir_deg;
    SGPropertyNode *true_airspeed_kt;
    SGPropertyNode *est_wind_target_heading_deg;

    // FIXME REMOVE!
    // console/logging property nodes
    SGPropertyNode *ap_console_skip;
    SGPropertyNode *ap_logging_skip;

    // bool home_set;
    bool msl_override;
    bool agl_override;

    // fgRouteMode mode;

    SGWayPoint make_waypoint( const string& target );

    bool build();

public:

    FGRouteMgr();
    ~FGRouteMgr();

    void bind();

    void init( SGPropertyNode *branch );

    void update();

    int new_waypoint( const string& tgt_alt );
    void add_waypoint( const SGWayPoint& wp );
    void replace_waypoint( const SGWayPoint &wp, int n );

    SGWayPoint get_waypoint( int i ) const {
        return route->get_waypoint(i);
    }

    SGWayPoint pop_waypoint( int i = 0 );

    int size() const {
        return route->size();
    }

    bool reposition_pattern( const SGWayPoint &wp, const double hdg );
};


#endif // _ROUTE_MGR_HXX
