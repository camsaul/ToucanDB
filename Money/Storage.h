//
//  Storage.h
//  Money
//
//  Created by Cam Saul on 8/28/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#pragma once

namespace Storage {
	istring Get		(istring key, bool* found);
	void	Set		(istring key, istring val);
	void	Delete	(istring key);
};
