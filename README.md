# FlipClock

A train station-style flip clock watch face for the Pebble smart watch.

**NOTE:** Requires Pebble Firmware v2.0 or greater.

### Configuration

The app is configurable via the Pebble smartphone app. Options include:
  * 12-hour time mode - Displays time using 12H format rather than 24H format.
  * Inverted colors - Black text on white screen rather than white text on black screen
  * Date position - The date can be displayed above the time, below the time, or
    not at all, in which case the time is displayed in the center of the screen.

### Screenshots

Example watchfaces in various states:

[![Screenshot showing normal, "first run" state (time is 18:25)](https://s3.amazonaws.com/pebble.rexmac.com/flipclock/screenshot1.png)](https://s3.amazonaws.com/pebble.rexmac.com/flipclock/screenshot1.png)&nbsp;
[![Screenshot showing time-only display (time is 18:26)](https://s3.amazonaws.com/pebble.rexmac.com/flipclock/screenshot2.png)](https://s3.amazonaws.com/pebble.rexmac.com/flipclock/screenshot2.png)&nbsp;
[![Screenshot showing date below time display (time is 18:25)](https://s3.amazonaws.com/pebble.rexmac.com/flipclock/screenshot3.png)](https://s3.amazonaws.com/pebble.rexmac.com/flipclock/screenshot3.png)&nbsp;
[![Screenshot showing inverted colors display (time is 18:25)](https://s3.amazonaws.com/pebble.rexmac.com/flipclock/screenshot4.png)](https://s3.amazonaws.com/pebble.rexmac.com/flipclock/screenshot4.png)

## Build

To build this watchface from source. Follow these steps:

1. Clone this repository in an appropriate directory. For example:

    `$ git clone https://github.com/rexmac/pebble-flipclock.git`

2. Build the project:

    `$ pebble build`

3. Install the compiled project to your Pebble:

    `$ pebble install --phone [your_phone_ip_here]`

For more information on building and installing Pebble apps from source, please see the official [Getting Started](https://developer.getpebble.com/2/getting-started/) guide.

## Bugs, Suggestions, Comments

Please use the [Github issue system](https://github.com/rexmac/pebble-flipclock/issues) to report bugs, request new features, or ask questions.

## Credits

Animation images and logic by [Rex McConnell](http://github.com/rexmac).

## License

Copyright (c) 2013, Japheth Thomson. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of the {organization} nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
