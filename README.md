#chordRecognition

This is my first own opensource project. This is my thesis.
This project is a chord recognition program what use this opensoruce project stuffs:
    http://ifs.tuwien.ac.at/mir/chorddetection.html
Changes:
* The original program is use libsndfile, but I wrote my own soundData and I linked it.
* The core is Qt with QtMultimedia.
* It uses music chordDatabase:
    audio/jim2012Chords/
    but I used wave files under the audio directory
* It use Neural Network for database search.
* ... ?

================

###Qt Multimedia

You’ll need at least alsa-lib (>= 1.0.15) and gstreamer (>=0.10.24, but <1.0 for now [lists.qt-project.org]) with the base-plugins package.
Ubuntu/Debian:

    sudo apt-get install libasound2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
    sudo apt-get install qtmobility-dev libprotobuf-dev protobuf-compiler libqt4-dev


###linear algebra libary

    http://arma.sourceforge.net/download.html



###fftw 

    http://www.fftw.org/download.html

###Git

    http://classic.scottr.org/presentations/git-in-5-minutes/


###sox

    sox --i sample.wav
    http://www.krisyu.org/blog/posts/2013/06/processing-audio-files-sox/#display


###problem: -1: error: cannot find -lGL qt

solved:

    sudo apt-get install mesa-common-dev
    sudo apt-get install freeglut3-dev

