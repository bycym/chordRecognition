chordRecognition
================

Qt Multimedia

You’ll need at least alsa-lib (>= 1.0.15) and gstreamer (>=0.10.24, but <1.0 for now [lists.qt-project.org]) with the base-plugins package.
Ubuntu/Debian:

    sudo apt-get install libasound2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
sudo apt-get install qtmobility-dev libprotobuf-dev protobuf-compiler libqt4-dev

fftw 

http://www.fftw.org/download.html

Git

http://classic.scottr.org/presentations/git-in-5-minutes/


sox:
sox --i sample.wav
http://www.krisyu.org/blog/posts/2013/06/processing-audio-files-sox/#display


Qt Creator:
Projects -> Build&Run
	Desktop -> Changekit -> Desktop Qt 5.4.1 GCCC
	*Change build directory*
	BuildStep (>Details<) -> qmake build conf. : Debug


problem: -1: error: cannot find -lGL qt
solved:
sudo apt-get install mesa-common-dev
sudo apt-get install freeglut3-dev
