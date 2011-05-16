#!/bin/sh

TTY="vt5"

if [ -f halo ]
then

    echo "Starting halo on tty $TTY"
    xinit ./halo -display :1 -- :1 $TTY

else

    echo "Halo file does not exist. Forgot to compile?"

fi
