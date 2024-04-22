#!/bin/bash
echo "FakeServer will start in new terminal."
# Start the first executable in a new console and keep it running in the background

gnome-terminal -- bash -c "./cmake-build-debug/test/FakeServer/FakeServer; read -p 'Press Enter to kill executable1' "

echo "FakeClient will start in this terminal."

#We need give to server start a bit.
sleep 1

# Start the second executable in the same console
bash -c ./cmake-build-debug/test/FakeClient/FakeClient




