#!/bin/bash -efu

enable -f ./timing enable_timeofday
enable_timeofday

echo "${TIMEOFDAY}"
date '+%s%N'