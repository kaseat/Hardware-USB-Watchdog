// Copyright 2017 Oleg Petrochenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

using System;
using HwdgWrapper;

namespace HwdgGui.Utils
{
    public interface ISettingsProvider
    {
        /// <summary>
        /// Can application start with Windows.
        /// </summary>
        Boolean Autorun { get; set; }

        /// <summary>
        /// Is monitoring sarts when application starts.
        /// </summary>
        Boolean Automonitor { get; set; }

        /// <summary>
        /// Save or restore hwdg status.
        /// </summary>
        Status HwdgStatus { get; set; }
    }
}