# SOBClock
Arduino Binary Clock - Using ring LEDs to show DOY and UTC time for satellite observing
This is my first repository, and my first crack at an arduino project, go easy on me.
Open to suggestions for cleaning up my code, it works, but is rather to verbose, in all the wrong ways.

This Project uses an Arduino, 2 x 12 segment LED rings, and an RTC clock to show:

DOY (Day of Year) - (LH Ring, 9 segments showing 1,2,4,8,16,32,64,128,256 days of the year) 

UTC Time - (RH Ring - 5 segments on top showing 1,2,4,8,16 in 24hr time, bottom 6 showing 1,2,4,8,16,32 minutes)

There's still a lot do do, see issues.
RTC is currently a DS3231.
LED Rings are currently 6812 RGBW 12 segment rings.

Regards,
Sandy - sandyjonesmusic@gmail.com
