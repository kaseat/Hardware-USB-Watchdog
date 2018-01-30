// Copyright 2018 Oleg Petrochenko
// 
// This file is part of Hwdg.
// 
// Hwdg is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// Hwdg is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Hwdg. If not, see <http://www.gnu.org/licenses/>.

#pragma once

class IExtiInterruptable
{
public:
	virtual void OnExtiInterrupt() = 0;

protected:
	virtual ~IExtiInterruptable()
	{
	}
};
