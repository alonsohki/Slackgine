/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        filter.h
// PURPOSE:     Strategy to filter certain component types.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include <vector>
#include <string>
#include "l3m/l3m.h"

extern bool filter_types ( l3m::Model* model, const std::vector < std::string >& types );
