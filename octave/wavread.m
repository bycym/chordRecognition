function [z,Fs,Format]=wavread(wavefile,start,length)
%WAVREAD Load Microsoft Windows 3.1 .WAV format sound files.
% [z]=WAVREAD(wavefile) loads a .WAV format file specified by "wavefile",
% returning the sampled data in variable "y". The .WAV extension
% in the filename is optional.
%
% [z,Fs]=WAVREAD(wavefile) loads a .WAV format file specified by
% "wavefile", returning the sampled data in variable "y" and the
% sample rate in variable "Fs".
%
% [z,Fs,Format]=WAVREAD(wavefile) loads a .WAV format file specified by
% "wavefile",returning the sampled data in variable "y", the sample
% rate in variable "Fs", and the .WAV file format information in
% variable "Format". The format information is returned as a 7 element
% vector with the following order:
%
% Format(1) Data format (always PCM)
% Format(2) Number of channels
% Format(3) Sample Rate (Fs)
% Format(4) Average bytes per second (sampled)
% Format(5) Block alignment of data
% Format(6) Bits per sample
% Format(7) Number of samples (total) --- new at 10-6-96
%
% The first optional parameter is start, the offset into the sample (in
% samples not bytes) , from which to start reading. (0 is the default.)
% The second optional parameter is length, the length (in samples) to
% read. (5 million is the default = 1.88 minutes.)
%
% Note: This is an extended version of WAVREAD (modified by Paul Masri [14-6-96])
% to read 8- or 16-bit, mono or stereo channel data. In the case of
% 2-channel data, each channel is in a different column of z (left in 1st).
%
% See also WAVWRITE.

% Copyright (c) 1984-94 by The MathWorks, Inc. (of original version)
% Copyright (c) 1996 by Paul Masri, Digital Music Research Group,
% University of Bristol (of modified version)
% http://www.fen.bris.ac.uk/elec/dmr

% Check parameters and assign defaults
if (nargin<1 | nargin>3)
error('WAVREAD takes between one and three arguments');
elseif (nargin<3)
length=5000000; % 5 million is just under 2 mins
end
if (nargin==1)
start=0;
end
if isempty(findstr(wavefile,'.'))
wavefile=[wavefile,'.wav'];
end

%Read the file
fid=fopen(wavefile,'rb','l');
if fid == -1
error('Can''t open .WAV file for input!');
end

% read riff chunk
header=fread(fid,4,'uchar'); % 'RIFF'
header=fread(fid,1,'ulong'); % Length+36
header=fread(fid,4,'uchar'); % WAVE

% read format sub-chunk
header=fread(fid,4,'uchar'); % 'fmt '
header=fread(fid,1,'ulong'); % ???


Format(1)=fread(fid,1,'ushort'); % Format tag (=1 for PCM)
if Format(1) ~= 1
error('WAVREAD only supports true PCM samples');
end

nchan=fread(fid,1,'ushort'); % No. channels
Format(2)=nchan;
if nchan > 2
error('WAVREAD supports mono or stereo samples only');
end

Fs=fread(fid,1,'ulong'); % Samples per second
Format(3)=Fs;

Format(4)=fread(fid,1,'ulong'); % average bytes per second

blocklen=fread(fid,1,'ushort'); % block alignment
Format(5)=blocklen;

Format(6)=fread(fid,1,'ushort'); % bits per sample
resbyte=Format(6)/8; % =bytes per sample
if resbyte==1
readtype='uchar';
else
readtype='short';
end

header=fread(fid,4,'uchar'); % 'data'
nbytes=fread(fid,1,'ulong'); % No. bytes in sample
nsamples=nbytes/blocklen; % = No. samples
Format(7)=nsamples;

if nsamples<=start
error('Cannot read - start point is beyond file end');
end
length=min(length,nsamples-start); % Truncate if necessary
if (fseek(fid,start*blocklen,'cof')==-1) % Skip samples before start
warning(ferror(fid,'clear'));
end

% Read the data
y=fread(fid,length*nchan,readtype);
fclose(fid);

% Split the channels if necessary
if nchan==1
z=y;
else
z(1:length,1:2)=[y(1:2:length*nchan) y(2:2:length*nchan)];
end