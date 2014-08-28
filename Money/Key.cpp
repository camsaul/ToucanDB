//
//  Key.cpp
//  Money
//
//  Created by Cam Saul on 8/24/14.
//  Copyright (c) 2014 Cam Saul. All rights reserved.
//

#include "Key.h"

const char * const Key::kValidChars = "+./_0123456789abcdefghijklmnopqrstuvwxyz";
int* Key::kCharToIntMap = nullptr;
char* Key::kIntToCharMap = nullptr;
