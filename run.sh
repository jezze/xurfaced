#!/bin/sh

TTY="vt5"

if [ -f xurfaced ]
then

    echo "Starting xurfaced on tty $TTY"
    xinit ./xurfaced -display :1 -- :1 $TTY

else

    echo "xurfaced file does not exist. Forgot to compile?"

fi
