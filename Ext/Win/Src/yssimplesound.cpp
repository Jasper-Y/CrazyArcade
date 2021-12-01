#include <algorithm>

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "yssimplesound.h"

YsSoundPlayer *YsSoundPlayer::currentPlayer=nullptr;

YsSoundPlayer::YsSoundPlayer()
{
	api=CreateAPISpecificData();
	playerStatePtr.reset(new STATE);
	*playerStatePtr=STATE_UNINITIALIZED;
}
YsSoundPlayer::~YsSoundPlayer()
{
	End();
	if(GetCurrentPlayer()==this)
	{
		NullifyCurrentPlayer();
	}
	DeleteAPISpecificData(api);
}

void YsSoundPlayer::MakeCurrent(void)
{
	currentPlayer=this;
}

/* static */ void YsSoundPlayer::NullifyCurrentPlayer(void)
{
	currentPlayer=nullptr;
}
/* static */ YsSoundPlayer *YsSoundPlayer::GetCurrentPlayer(void)
{
	return currentPlayer;
}



////////////////////////////////////////////////////////////



YsSoundPlayer::SoundData::MemInStream::MemInStream(long long int len,const unsigned char dat[])
{
	this->length=len;
	this->pointer=0;
	this->dat=dat;
}
long long int YsSoundPlayer::SoundData::MemInStream::Fetch(unsigned char buf[],long long int len)
{
	long long int nCopy=0;
	if(pointer+len<length)
	{
		nCopy=len;
	}
	else
	{
		nCopy=length-pointer;
	}
	for(long long int i=0; i<nCopy; ++i)
	{
		buf[i]=dat[pointer];
		++pointer;
	}
	return nCopy;
}

long long int YsSoundPlayer::SoundData::MemInStream::Skip(long long int len)
{
	long long int nSkip=0;
	if(pointer+len<length)
	{
		nSkip=len;
	}
	else
	{
		nSkip=length-pointer;
	}
	pointer+=nSkip;
	return nSkip;
}

YsSoundPlayer::SoundData::FileInStream::FileInStream(FILE *fp)
{
	this->fp=fp;
}
long long int YsSoundPlayer::SoundData::FileInStream::Skip(long long int len)
{
	size_t totalSkipped=0;

	const long long skipBufSize=1024*1024;
	char *skipbuf=new char [skipBufSize];
	while(0<len)
	{
		auto toSkip=len;
		if(skipBufSize<toSkip)
		{
			toSkip=skipBufSize;
		}
		auto skipped=fread(skipbuf,1,toSkip,fp);
		totalSkipped+=skipped;
		len-=skipped;
		if(0==skipped)
		{
			break;
		}
	}
	delete [] skipbuf;
	return totalSkipped;
}
long long int YsSoundPlayer::SoundData::FileInStream::Fetch(unsigned char buf[],long long int len)
{
	return fread(buf,1,len,fp);
}

////////////////////////////////////////////////////////////

void YsSoundPlayer::Start(void)
{
	if(YSOK==StartAPISpecific())
	{
		*playerStatePtr=STATE_STARTED;
	}
}
void YsSoundPlayer::End(void)
{
	EndAPISpecific();
	*playerStatePtr=STATE_ENDED;
}

void YsSoundPlayer::PreparePlay(SoundData &dat)
{
	if(nullptr==dat.playerStatePtr)
	{
		dat.playerStatePtr=this->playerStatePtr;
	}
	else if(dat.playerStatePtr!=this->playerStatePtr)
	{
		printf("%s %d\n",__FUNCTION__,__LINE__);
		printf("  YsSoundPlayer::SoundData can be associated with only one player.\n");
		return;
	}
	dat.PreparePlay(*this);
	dat.prepared=true;
}

void YsSoundPlayer::PlayOneShot(SoundData &dat)
{
	if(true!=dat.prepared)
	{
		PreparePlay(dat);
	}
	if(YSOK==PlayOneShotAPISpecific(dat))
	{
	}
}
void YsSoundPlayer::PlayBackground(SoundData &dat)
{
	if(true!=dat.prepared)
	{
		PreparePlay(dat);
	}
	if(YSOK==PlayBackgroundAPISpecific(dat))
	{
	}
}

YSRESULT YsSoundPlayer::StartStreaming(Stream &streamPlayer)
{
	return StartStreamingAPISpecific(streamPlayer);
}

void YsSoundPlayer::StopStreaming(Stream &streamPlayer)
{
	StopStreamingAPISpecific(streamPlayer);
}

YSBOOL YsSoundPlayer::StreamPlayerReadyToAcceptNextSegment(const Stream &streamPlayer,const SoundData &dat) const
{
	// Currently not supporting 8 bit samples.
	if(16!=dat.BitPerSample())
	{
		return YSFALSE;
	}
	return StreamPlayerReadyToAcceptNextSegmentAPISpecific(streamPlayer,dat);
}

YSRESULT YsSoundPlayer::AddNextStreamingSegment(Stream &streamPlayer,const SoundData &dat)
{
	// Currently not supporting 8 bit samples.
	if(16!=dat.BitPerSample())
	{
		return YSERR;
	}
	return AddNextStreamingSegmentAPISpecific(streamPlayer,dat);
}

void YsSoundPlayer::Stop(SoundData &dat)
{
	StopAPISpecific(dat);
}
void YsSoundPlayer::Pause(SoundData &dat)
{
	PauseAPISpecific(dat);
}
void YsSoundPlayer::Resume(SoundData &dat)
{
	ResumeAPISpecific(dat);
}

void YsSoundPlayer::KeepPlaying(void)
{
	KeepPlayingAPISpecific();
}

YSBOOL YsSoundPlayer::IsPlaying(const SoundData &dat) const
{
	return IsPlayingAPISpecific(dat);
}

double YsSoundPlayer::GetCurrentPosition(const SoundData &dat) const
{
	return GetCurrentPositionAPISpecific(dat);
}

void YsSoundPlayer::SetVolume(SoundData &dat,float vol)
{
	dat.playBackVolume=vol;
	SetVolumeAPISpecific(dat,vol);
}

////////////////////////////////////////////////////////////



YsSoundPlayer::SoundData::SoundData()
{
	api=CreateAPISpecificData();
	Initialize();
}

YsSoundPlayer::SoundData::~SoundData()
{
	CleanUp();
	DeleteAPISpecificData(api);
}

void YsSoundPlayer::SoundData::CopyFrom(const SoundData &incoming)
{
	if(this!=&incoming)
	{
		CleanUp();

		prepared=false;

		playerStatePtr=incoming.playerStatePtr;

		lastModifiedChannel=incoming.lastModifiedChannel;
		stereo=incoming.stereo;
		bit=incoming.bit;
		rate=incoming.rate;
		sizeInBytes=incoming.sizeInBytes;

		isSigned=incoming.isSigned;
		dat=incoming.dat;
		playBackVolume=incoming.playBackVolume;

		// Do not copy -> APISpecificDataPerSoundData *api;
	}
}

void YsSoundPlayer::SoundData::MoveFrom(SoundData &incoming)
{
	if(this!=&incoming)
	{
		CleanUp();

		prepared=false;

		playerStatePtr=incoming.playerStatePtr;

		lastModifiedChannel=incoming.lastModifiedChannel;
		stereo=incoming.stereo;
		bit=incoming.bit;
		rate=incoming.rate;
		sizeInBytes=incoming.sizeInBytes;

		isSigned=incoming.isSigned;
		std::swap(dat,incoming.dat);
		playBackVolume=incoming.playBackVolume;

		incoming.CleanUp();

		// Do not copy -> APISpecificDataPerSoundData *api;
	}
}

void YsSoundPlayer::SoundData::Initialize(void)
{
	CleanUp();
}

void YsSoundPlayer::SoundData::CleanUp(void)
{
	CleanUpAPISpecific();

	dat.clear();

	prepared=false;

	lastModifiedChannel=0;
	stereo=YSFALSE;
	bit=16;
	rate=44100;
	sizeInBytes=0;
	isSigned=YSTRUE;
	playBackVolume=1.0;
}

unsigned int YsSoundPlayer::SoundData::NTimeStep(void) const
{
	return SizeInByte()/BytePerTimeStep();
}

YSBOOL YsSoundPlayer::SoundData::Stereo(void) const
{
	return stereo;
}

unsigned int YsSoundPlayer::SoundData::BytePerTimeStep(void) const
{
	unsigned int nChannel=(YSTRUE==stereo ? 2 : 1);
	return nChannel*BytePerSample();
}

unsigned int YsSoundPlayer::SoundData::BitPerSample(void) const
{
	return bit;
}

unsigned int YsSoundPlayer::SoundData::BytePerSample(void) const
{
	return bit/8;
}

long long YsSoundPlayer::SoundData::SecToNumSample(double sec) const
{
	return SecToNumSample(sec,PlayBackRate());
}
/* static */ long long YsSoundPlayer::SoundData::SecToNumSample(double sec,unsigned int playBackRate)
{
	return (long long)(sec*(double)playBackRate);
}
double YsSoundPlayer::SoundData::NumSampleToSec(long long numSample) const
{
	return NumSampleToSec(numSample,PlayBackRate());
}
/* static */ double YsSoundPlayer::SoundData::NumSampleToSec(long long numSample,unsigned int playBackRate)
{
	return (double)numSample/(double)playBackRate;
}

unsigned int YsSoundPlayer::SoundData::PlayBackRate(void) const
{
	return rate;
}

unsigned int YsSoundPlayer::SoundData::SizeInByte(void) const
{
	return sizeInBytes;
}

YSBOOL YsSoundPlayer::SoundData::IsSigned(void) const
{
	return isSigned;
}

const unsigned char *YsSoundPlayer::SoundData::DataPointer(void) const
{
	return dat.data();
}

const unsigned char *YsSoundPlayer::SoundData::DataPointerAtTimeStep(unsigned int ts) const
{
	return dat.data()+ts*BytePerTimeStep();
}

static unsigned GetUnsigned(const unsigned char buf[])
{
	return buf[0]+buf[1]*0x100+buf[2]*0x10000+buf[3]*0x1000000;
}

static unsigned GetUnsignedShort(const unsigned char buf[])
{
	return buf[0]+buf[1]*0x100;
}



YSRESULT YsSoundPlayer::SoundData::CreateFromSigned16bitStereo(unsigned int samplingRate,std::vector <unsigned char> &wave)
{
	CleanUp();

	stereo=YSTRUE;
	bit=16;
	rate=samplingRate;
	sizeInBytes=wave.size();
	isSigned=YSTRUE;
	playBackVolume=1.0;

	dat.swap(wave);

	return YSOK;
}



YSRESULT YsSoundPlayer::SoundData::LoadWav(const char fn[])
{
	FILE *fp;
	printf("Loading %s\n",fn);
	fp=fopen(fn,"rb");
	if(fp!=NULL)
	{
		auto res=LoadWav(fp);
		fclose(fp);
		return res;
	}
	return YSERR;
}

YSRESULT YsSoundPlayer::SoundData::LoadWav(FILE *fp)
{
	if(nullptr!=fp)
	{
		FileInStream inStream(fp);
		return LoadWav(inStream);
	}
	return YSERR;
}

YSRESULT YsSoundPlayer::SoundData::LoadWavFromMemory(long long int length,const unsigned char incoming[])
{
	MemInStream inStream(length,incoming);
	return LoadWav(inStream);
}

YSRESULT YsSoundPlayer::SoundData::LoadWav(BinaryInStream &inStream)
{
	CleanUp();

	unsigned char buf[256];
	unsigned int l;
	unsigned int fSize,hdrSize,dataSize;

	// Wave Header
	unsigned short wFormatTag,nChannels;
	unsigned nSamplesPerSec,nAvgBytesPerSec;
	unsigned short nBlockAlign,wBitsPerSample,cbSize;


	if(inStream.Fetch(buf,4)!=4)
	{
		printf("Error in reading RIFF.\n");
		return YSERR;
	}
	if(strncmp((char *)buf,"RIFF",4)!=0)
	{
		printf("Warning: RIFF not found.\n");
	}


	if(inStream.Fetch(buf,4)!=4)
	{
		printf("Error in reading file size.\n");
		return YSERR;
	}
	fSize=GetUnsigned(buf);
	printf("File Size=%d\n",fSize+8);
	// Wait, is it fSize+12?  A new theory tells that "fmt " immediately following "WAVE"
	// is a chunk???

	if(inStream.Fetch(buf,8)!=8)
	{
		printf("Error in reading WAVEfmt.\n");
		return YSERR;
	}
	if(strncmp((char *)buf,"WAVEfmt",7)!=0)
	{
		printf("Warning: WAVEfmt not found\n");
	}


	if(inStream.Fetch(buf,4)!=4)
	{
		printf("Error in reading header size.\n");
		return YSERR;
	}
	hdrSize=GetUnsigned(buf);
	printf("Header Size=%d\n",hdrSize);


	//    WORD  wFormatTag; 
	//    WORD  nChannels; 
	//    DWORD nSamplesPerSec; 
	//    DWORD nAvgBytesPerSec; 
	//    WORD  nBlockAlign; 
	//    WORD  wBitsPerSample; 
	//    WORD  cbSize; 
	if(inStream.Fetch(buf,hdrSize)!=hdrSize)
	{
		printf("Error in reading header.\n");
		return YSERR;
	}
	wFormatTag=GetUnsignedShort(buf);
	nChannels=GetUnsignedShort(buf+2);
	nSamplesPerSec=GetUnsigned(buf+4);
	nAvgBytesPerSec=GetUnsigned(buf+8);
	nBlockAlign=GetUnsignedShort(buf+12);
	wBitsPerSample=(hdrSize>=16 ? GetUnsignedShort(buf+14) : 0);
	cbSize=(hdrSize>=18 ? GetUnsignedShort(buf+16) : 0);

	printf("wFormatTag=%d\n",wFormatTag);
	printf("nChannels=%d\n",nChannels);
	printf("nSamplesPerSec=%d\n",nSamplesPerSec);
	printf("nAvgBytesPerSec=%d\n",nAvgBytesPerSec);
	printf("nBlockAlign=%d\n",nBlockAlign);
	printf("wBitsPerSample=%d\n",wBitsPerSample);
	printf("cbSize=%d\n",cbSize);



	for(;;)
	{
		if(inStream.Fetch(buf,4)!=4)
		{
			printf("Error while waiting for data.\n");
			return YSERR;
		}

		if((buf[0]=='d' || buf[0]=='D') && (buf[1]=='a' || buf[1]=='A') &&
		   (buf[2]=='t' || buf[2]=='T') && (buf[3]=='a' || buf[3]=='A'))
		{
			break;
		}
		else
		{
			printf("Skipping %c%c%c%c (Unknown Block)\n",buf[0],buf[1],buf[2],buf[3]);
			if(inStream.Fetch(buf,4)!=4)
			{
				printf("Error while skipping unknown block.\n");
				return YSERR;
			}



			l=GetUnsigned(buf);
			if(inStream.Skip(l)!=l)
			{
				printf("Error while skipping unknown block.\n");
				return YSERR;
			}
		}
	}


	if(inStream.Fetch(buf,4)!=4)
	{
		printf("Error in reading data size.\n");
		return YSERR;
	}
	dataSize=GetUnsigned(buf);
	printf("Data Size=%d (0x%x)\n",dataSize,dataSize);

	dat.resize(dataSize);
	if((l=inStream.Fetch(dat.data(),dataSize))!=dataSize)
	{
		printf("Warning: File ended before reading all data.\n");
		printf("  %d (0x%x) bytes have been read\n",l,l);
	}

	this->stereo=(nChannels==2 ? YSTRUE : YSFALSE);
	this->bit=wBitsPerSample;
	this->sizeInBytes=dataSize;
	this->rate=nSamplesPerSec;

	if(wBitsPerSample==8)
	{
		isSigned=YSFALSE;
	}
	else
	{
		isSigned=YSTRUE;
	}

	return YSOK;
}


YSRESULT YsSoundPlayer::SoundData::ConvertTo16Bit(void)
{
	if(bit==16)
	{
		return YSOK;
	}
	else if(bit==8)
	{
		prepared=false;
		if(sizeInBytes>0 && 0<dat.size()) // ? Why did I write 0<dat.size()?  2020/04/02
		{
			std::vector <unsigned char> newDat;
			newDat.resize(sizeInBytes*2);
			for(int i=0; i<sizeInBytes; i++)
			{
				newDat[i*2]  =dat[i];
				newDat[i*2+1]=dat[i];
			}
			std::swap(dat,newDat);

			sizeInBytes*=2;
			bit=16;
		}
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsSoundPlayer::SoundData::ConvertTo8Bit(void)
{
	if(bit==8)
	{
		return YSOK;
	}
	else if(bit==16)
	{
		prepared=false;
		std::vector <unsigned char> newDat;
		newDat.resize(sizeInBytes/2);
		for(int i=0; i<sizeInBytes; i+=2)
		{
			newDat[i/2]=dat[i];
		}
		std::swap(dat,newDat);
		bit=8;
		sizeInBytes/=2;
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsSoundPlayer::SoundData::ConvertToStereo(void)
{
	if(stereo==YSTRUE)
	{
		return YSOK;
	}
	else
	{
		if(bit==8)
		{
			std::vector <unsigned char> newDat;
			newDat.resize(sizeInBytes*2);
			for(int i=0; i<sizeInBytes; i++)
			{
				newDat[i*2  ]=dat[i];
				newDat[i*2+1]=dat[i];
			}
			std::swap(dat,newDat);
			stereo=YSTRUE;
			sizeInBytes*=2;
			return YSOK;
		}
		else if(bit==16)
		{
			std::vector <unsigned char> newDat;
			newDat.resize(sizeInBytes*2);
			for(int i=0; i<sizeInBytes; i+=2)
			{
				newDat[i*2  ]=dat[i];
				newDat[i*2+1]=dat[i+1];
				newDat[i*2+2]=dat[i];
				newDat[i*2+3]=dat[i+1];
			}
			std::swap(dat,newDat);
			stereo=YSTRUE;
			sizeInBytes*=2;
			return YSOK;
		}
	}
	return YSERR;
}

YSRESULT YsSoundPlayer::SoundData::Resample(int newRate)
{
	if(rate!=newRate)
	{
		prepared=false;

		const size_t nChannel=(YSTRUE==stereo ? 2 : 1);
		const size_t bytePerSample=bit/8;
		const size_t bytePerTimeStep=nChannel*bytePerSample;
		const size_t curNTimeStep=sizeInBytes/bytePerTimeStep;

		const size_t newNTimeStep=curNTimeStep*newRate/rate;
		const size_t newSize=newNTimeStep*bytePerTimeStep;

		std::vector <unsigned char> newDat;
		newDat.resize(newSize);
		{
			for(size_t ts=0; ts<newNTimeStep; ts++)
			{
				double oldTimeStepD=(double)curNTimeStep*(double)ts/(double)newNTimeStep;
				size_t oldTimeStep=(size_t)oldTimeStepD;
				double param=fmod(oldTimeStepD,1.0);
				unsigned char *newTimeStepPtr=newDat.data()+ts*bytePerTimeStep;

				for(size_t ch=0; ch<nChannel; ++ch)
				{
					if(curNTimeStep-1<=oldTimeStep)
					{
						const int value=GetSignedValueRaw(ch,curNTimeStep-1);
						SetSignedValueRaw(newTimeStepPtr+bytePerSample*ch,value);
					}
					else if(0==oldTimeStep || curNTimeStep-2<=oldTimeStep)
					{
						const double value[2]=
							{
								(double)GetSignedValueRaw(ch,oldTimeStep),
								(double)GetSignedValueRaw(ch,oldTimeStep+1)
							};
						const int newValue=(int)(value[0]*(1.0-param)+value[1]*param);
						SetSignedValueRaw(newTimeStepPtr+bytePerSample*ch,newValue);
					}
					else
					{
						const double v[4]=
							{
								(double)GetSignedValueRaw(ch,oldTimeStep-1),  // At x=-1.0
								(double)GetSignedValueRaw(ch,oldTimeStep),    // At x= 0.0
								(double)GetSignedValueRaw(ch,oldTimeStep+1),  // At x= 1.0
								(double)GetSignedValueRaw(ch,oldTimeStep+2)   // At x= 2.0
							};

						// Cubic interpolation.  Linear didn't work well.
						// axxx+bxx+cx+d=e
						// x=-1  -> -a+b-c+d=v0   (A)
						// x= 0  ->        d=v1   (B)
						// x= 1  ->  a+b+c+d=v2   (C)
						// x= 2  -> 8a+4b+2c+d=v3 (D)
						//
						// (B) =>  d=v1;
						// (A)+(C) => 2b+2d=v0+v2  => b=(v0+v2-2d)/2
						//
						// (D)-2*(B) =>  6a+2b-d=v3-2*v2
						//           =>  a=(v3-2*v2-2b+d)/6

						const double d=v[1];
						const double b=(v[0]+v[2]-2.0*d)/2.0;
						const double a=(v[3]-2.0*v[2]-2.0*b+d)/6.0;
						const double c=v[2]-a-b-d;

						double newValue=a*param*param*param
							+b*param*param
							+c*param
							+d;
						SetSignedValueRaw(newTimeStepPtr+bytePerSample*ch,(int)newValue);
					}
				}
			}
		}

		rate=newRate;
		std::swap(dat,newDat);
		sizeInBytes=newSize;
	}
	return YSOK;
}

YSRESULT YsSoundPlayer::SoundData::ConvertToMono(void)
{
	if(YSTRUE==stereo)
	{
		prepared=false;

		const size_t bytePerSample=bit/8;
		const size_t bytePerTimeStep=2*bytePerSample;
		const size_t nTimeStep=sizeInBytes/bytePerTimeStep;

		const size_t newSize=nTimeStep*bytePerSample;

		std::vector <unsigned char> newDat;
		newDat.resize(newSize);
		{
			for(size_t ts=0; ts<nTimeStep; ts++)
			{
				const int newValue=(GetSignedValueRaw(0,ts)+GetSignedValueRaw(1,ts))/2;
				unsigned char *const newTimeStepPtr=newDat.data()+ts*bytePerSample;
				SetSignedValueRaw(newTimeStepPtr,newValue);
			}

			std::swap(dat,newDat);
			sizeInBytes=newSize;
			stereo=YSFALSE;

			return YSOK;
		}
	}
	return YSERR;
}

YSRESULT YsSoundPlayer::SoundData::ConvertToSigned(void)
{
	if(isSigned==YSTRUE)
	{
		return YSOK;
	}
	else
	{
		prepared=false;
		if(bit==8)
		{
			for(int i=0; i<sizeInBytes; i++)
			{
				dat[i]-=128;
			}
		}
		else if(bit==16)
		{
			for(int i=0; i<sizeInBytes-1; i+=2)
			{
				int d;
				d=dat[i]+dat[i+1]*256;
				d-=32768;
				dat[i]=d&255;
				dat[i+1]=(d>>8)&255;
			}
		}
		isSigned=YSTRUE;
	}
	return YSOK;
}

YSRESULT YsSoundPlayer::SoundData::ConvertToUnsigned(void)
{
	if(isSigned!=YSTRUE)
	{
		return YSOK;
	}
	else
	{
		prepared=false;
		if(bit==8)
		{
			for(int i=0; i<sizeInBytes; i++)
			{
				dat[i]+=128;
			}
		}
		else if(bit==16)
		{
			for(int i=0; i<sizeInBytes-1; i+=2)
			{
				int d=dat[i]+dat[i+1]*256;
				if(d>=32768)
				{
					d-=65536;
				}
				d+=32768;
				dat[i]=d&255;
				dat[i+1]=(d>>8)&255;
			}
		}
		isSigned=YSFALSE;
	}
	return YSOK;
}

YSRESULT YsSoundPlayer::SoundData::DeleteChannel(int channel)
{
	if(YSTRUE==stereo)
	{
		prepared=false;

		const size_t bytePerSample=bit/8;
		const size_t bytePerTimeStep=2*bytePerSample;
		const size_t nTimeStep=sizeInBytes/bytePerTimeStep;

		const size_t newSize=nTimeStep*bytePerSample;

		std::vector <unsigned char> newDat;
		newDat.resize(newSize);
		{
			for(size_t ts=0; ts<nTimeStep; ts++)
			{
				const int newValue=GetSignedValueRaw(1-channel,ts);
				unsigned char *const newTimeStepPtr=newDat.data()+ts*bytePerSample;
				SetSignedValueRaw(newTimeStepPtr,newValue);
			}

			std::swap(dat,newDat);
			sizeInBytes=newSize;
			stereo=YSFALSE;

			if(channel<=lastModifiedChannel)
			{
				lastModifiedChannel--;
				if(0>lastModifiedChannel)
				{
					lastModifiedChannel=0;
				}
			}

			return YSOK;
		}
	}
	return YSERR;
}

//int main(int ac,char *av[])
//{
//	YsWavFile test;
//	test.LoadWav(av[1]);
//	return 0;
//}

int YsSoundPlayer::SoundData::GetNumChannel(void) const
{
	return (YSTRUE==stereo ? 2 : 1);
}

int YsSoundPlayer::SoundData::GetLastModifiedChannel(void) const
{
	return lastModifiedChannel;
}

int YsSoundPlayer::SoundData::GetNumSample(void) const
{
	return (sizeInBytes*8/bit);
}

int YsSoundPlayer::SoundData::GetNumSamplePerChannel(void) const
{
	return GetNumSample()/GetNumChannel();
}

size_t YsSoundPlayer::SoundData::GetUnitSize(void) const
{
	return BytePerSample()*GetNumChannel();
}

size_t YsSoundPlayer::SoundData::GetSamplePosition(int atIndex) const
{
	return atIndex*GetNumChannel()*(bit/8);
}

int YsSoundPlayer::SoundData::GetSignedValueRaw(int channel,int atTimeStep) const
{
	const size_t sampleIdx=GetSamplePosition(atTimeStep);
	const size_t unitSize=GetUnitSize();

	if(sampleIdx+unitSize<=sizeInBytes && 0<=channel && channel<GetNumChannel())
	{
		int rawSignedValue=0;
		size_t offset=sampleIdx+channel*BytePerSample();
		switch(BitPerSample())
		{
		case 8:
			if(YSTRUE==isSigned)
			{
				rawSignedValue=dat[offset];
				if(128<=rawSignedValue)
				{
					rawSignedValue-=256;
				}
			}
			else
			{
				rawSignedValue=dat[offset]-128;
			}
			break;
		case 16:
			// Assume little endian
			rawSignedValue=dat[offset]+256*dat[offset+1];
			if(YSTRUE==isSigned)
			{
				if(32768<=rawSignedValue)
				{
					rawSignedValue-=65536;
				}
			}
			else
			{
				rawSignedValue-=32768;
		    }
			break;
		}
		return rawSignedValue;
	}
	return 0;
}

void YsSoundPlayer::SoundData::SetSignedValue16(int channel,int atTimeStep,int rawSignedValue)
{
	const size_t sampleIdx=GetSamplePosition(atTimeStep);
	const size_t unitSize=GetUnitSize();

	if(sampleIdx+unitSize<=sizeInBytes && 0<=channel && channel<GetNumChannel())
	{
		prepared=false;
		lastModifiedChannel=channel;
		size_t offset=sampleIdx+channel*BytePerSample();
		switch(BitPerSample())
		{
		case 8:
			if(YSTRUE==isSigned)
			{
				rawSignedValue>>=8;
				rawSignedValue&=255;
				dat[offset]=rawSignedValue;
			}
			else
			{
				rawSignedValue>>=8;
				rawSignedValue+=128;
				rawSignedValue&=255;
				dat[offset]=rawSignedValue;
			}
			break;
		case 16:
			// Assume little endian
			if(YSTRUE==isSigned)
			{
				dat[offset  ]=(rawSignedValue&255);
				dat[offset+1]=((rawSignedValue>>8)&255);
			}
			else
			{
				rawSignedValue+=32768;
				dat[offset  ]=(rawSignedValue&255);
				dat[offset+1]=((rawSignedValue>>8)&255);
		    }
			break;
		}
	}
}

int YsSoundPlayer::SoundData::GetSignedValue16(int channel,int atTimeStep) const
{
	if(16==bit)
	{
		return GetSignedValueRaw(channel,atTimeStep);
	}
	else if(8==bit)
	{
		auto value=GetSignedValueRaw(channel,atTimeStep);
		value=value*32767/127;
		return value;
	}
	return 0; // ?
}

void YsSoundPlayer::SoundData::ResizeByNumSample(long long int nSample)
{
	prepared=false;

	long long int newDataSize=nSample*GetNumChannel()*BytePerSample();
	std::vector <unsigned char> newDat;
	newDat.resize(newDataSize);

	for(long long int i=0; i<newDataSize && i<sizeInBytes; ++i)
	{
		newDat[i]=dat[i];
	}
	for(long long int i=sizeInBytes; i<newDataSize; ++i)
	{
		newDat[i]=0;
	}

	std::swap(dat,newDat);
	sizeInBytes=newDataSize;
}

void YsSoundPlayer::SoundData::SetSignedValueRaw(unsigned char *savePtr,int rawSignedValue)
{
	prepared=false;
	switch(bit)
	{
	case 8:
		if(rawSignedValue<-128)
		{
			rawSignedValue=-128;
		}
		else if(127<rawSignedValue)
		{
			rawSignedValue=127;
		}
		if(YSTRUE==isSigned)
		{
			if(0>rawSignedValue)
			{
				rawSignedValue+=256;
			}
			*savePtr=(unsigned char)rawSignedValue;
		}
		else
		{
			rawSignedValue+=128;
			*savePtr=(unsigned char)rawSignedValue;
		}
		break;
	case 16:
		if(-32768>rawSignedValue)
		{
			rawSignedValue=-32768;
		}
		else if(32767<rawSignedValue)
		{
			rawSignedValue=32767;
		}

		if(YSTRUE==isSigned)
		{
			if(0>rawSignedValue)
			{
				rawSignedValue+=65536;
			}
		}
		else
		{
			rawSignedValue+=32768;
		}

		// Assume little endian (.WAV is supposed to use little endian).
		savePtr[0]=(rawSignedValue&255);
		savePtr[1]=((rawSignedValue>>8)&255);
		break;
	}
}



std::vector <unsigned char> YsSoundPlayer::SoundData::MakeWavByteData(void) const
{
	std::vector <unsigned char> byteData;
	byteData.push_back('R');
	byteData.push_back('I');
	byteData.push_back('F');
	byteData.push_back('F');

	AddUnsignedInt(byteData,0);

	byteData.push_back('W');
	byteData.push_back('A');
	byteData.push_back('V');
	byteData.push_back('E');
	byteData.push_back('f');
	byteData.push_back('m');
	byteData.push_back('t');
	byteData.push_back(' ');

	const int nChannels=GetNumChannel();
	const int nBlockAlign=nChannels*BitPerSample()/8;
	const int nAvgBytesPerSec=PlayBackRate()*nBlockAlign;

	AddUnsignedInt(byteData,16);
	AddUnsignedShort(byteData,1); // wFormatTag=1
	AddUnsignedShort(byteData,(unsigned short)GetNumChannel());
	AddUnsignedInt(byteData,PlayBackRate());  // nSamplesPerSec
	AddUnsignedInt(byteData,nAvgBytesPerSec);
	AddUnsignedShort(byteData,(unsigned short)nBlockAlign);
	AddUnsignedShort(byteData,(unsigned short)BitPerSample()); // wBitsPerSample

	byteData.push_back('d');
	byteData.push_back('a');
	byteData.push_back('t');
	byteData.push_back('a');

	AddUnsignedInt(byteData,SizeInByte());

	for(int i=0; i<SizeInByte(); ++i)
	{
		byteData.push_back(dat[i]);
	}

	auto totalSize=byteData.size()-8;
	byteData[4]=totalSize&255;
	byteData[5]=(totalSize>>8)&255;
	byteData[6]=(totalSize>>16)&255;
	byteData[7]=(totalSize>>24)&255;

	return byteData;
}

/* static */ void YsSoundPlayer::SoundData::AddUnsignedInt(std::vector <unsigned char> &byteData,unsigned int dat)
{
	unsigned char buf[4];
	buf[0]=dat&255;
	buf[1]=(dat>>8)&255;
	buf[2]=(dat>>16)&255;
	buf[3]=(dat>>24)&255;
	byteData.push_back(buf[0]);
	byteData.push_back(buf[1]);
	byteData.push_back(buf[2]);
	byteData.push_back(buf[3]);
}

/* static */ void YsSoundPlayer::SoundData::AddUnsignedShort(std::vector <unsigned char> &byteData,unsigned short dat)
{
	unsigned char buf[2];
	buf[0]=dat&255;
	buf[1]=(dat>>8)&255;
	byteData.push_back(buf[0]);
	byteData.push_back(buf[1]);
}




YsSoundPlayer::Stream::Stream()
{
	api=CreateAPISpecificData();
}
YsSoundPlayer::Stream::~Stream()
{
	DeleteAPISpecificData(api);
	api=nullptr;
}


#include <algorithm>



#include <stdio.h>



#define _WINSOCKAPI_

#include <windows.h>

#include <mmsystem.h>

#include <dsound.h>



#include "yssimplesound.h"





#pragma comment(lib,"user32.lib")

#pragma comment(lib,"winmm.lib")

#pragma comment(lib,"kernel32.lib")

#pragma comment(lib,"dsound.lib")





#ifdef min

#undef min // ****ing windows.h namespace contamination.

#endif





class YsSoundPlayer::APISpecificData

{

public:

	/*!

	Why do I have to find a window handle?  It is because DirectSound API is so badly designed that it requires a window handle.

	What's even more stupid is that the window needs to be active and forward to play sound.  I cannot get away with creating a

	dummy hidden window.  After wasting days of google search, I concluded that DirectSound API is designed by an incompetent

	programmer.



	Window and sound need to be independent.  Sound can optionally be associated with a window, but the association must not be mandatory.

	Unrelated modules must be independent.  It is such a basic.



	I mostly write a single-window application.  Therefore I can live with it.  But, if there are multiple windows, the sound

	stops when a window that is associated with DirectSound becomes inactive.



	DirectSound is like an opps library.  It might really be designed by an inexperienced graduate student.  Who knows.

	*/

	class MainWindowFinder

	{

	public:

		/*! This function finds a handle of the largest window that is visible and is associated with the current process Id.

		    This window may not be an application main window.  But, what else can I do?

		    This function can be used for giving a window handle to a badly-designed APIs such as Direct Sound API.

		*/

		static HWND Find(HWND hExcludeWnd=nullptr);

	private:

		static void SearchTopLevelWindow(HWND &hWndLargest,int &largestWndArea,HWND hWnd,DWORD procId,HWND hWndExclude);

	};





	HWND hWndMain;

	HWND hOwnWnd;

	LPDIRECTSOUND8 dSound8;



	APISpecificData();

	~APISpecificData();

	void CleanUp(void);

	void Start(void);

	void End(void);



	void RefetchMainWindowHandle(void);



private:

	void OpenDummyWindow(void);

	void DestroyDummyWindow(void);

	static LRESULT WINAPI OwnWindowFunc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp);

};



class YsSoundPlayer::SoundData::APISpecificDataPerSoundData

{

public:

	LPDIRECTSOUNDBUFFER dSoundBuf;



	APISpecificDataPerSoundData();

	~APISpecificDataPerSoundData();

	void CleanUp(void);



	void CreateBuffer(LPDIRECTSOUND8 dSound8,SoundData &dat);

};







////////////////////////////////////////////////////////////







// Source: http://stackoverflow.com/questions/6202547/win32-get-main-wnd-handle-of-application

HWND YsSoundPlayer::APISpecificData::MainWindowFinder::Find(HWND hExcludeWnd)

{

	HWND hWndLargest=NULL;

	int wndArea=0;

	SearchTopLevelWindow(hWndLargest,wndArea,NULL,GetCurrentProcessId(),hExcludeWnd);

	return hWndLargest;

}

void YsSoundPlayer::APISpecificData::MainWindowFinder::SearchTopLevelWindow(HWND &hWndLargest,int &largestWndArea,HWND hWnd,DWORD procId,HWND hWndExclude)

{

	if(nullptr!=hWndExclude && hWnd==hWndExclude)

	{

		return;

	}



	DWORD windowProcId;

	GetWindowThreadProcessId(hWnd,&windowProcId);

	if(windowProcId==procId)

	{

		char str[256];

		GetWindowTextA(hWnd,str,255);

		printf("hWnd=%08x Title=%s\n",(int)hWnd,str);

		printf("IsVisible=%d\n",IsWindowVisible(hWnd));



		RECT rc;

		GetWindowRect(hWnd,&rc);



		int area=((rc.right-rc.left)*(rc.bottom-rc.top));

		if(0>area)

		{

			area=-area;

		}

		printf("Area=%d square pixels. (%d x %d)\n",area,(rc.right-rc.left),(rc.bottom-rc.top));



		if(TRUE==IsWindowVisible(hWnd) && (NULL==hWndLargest || largestWndArea<area))

		{

			hWndLargest=hWnd;

			largestWndArea=area;

		}

		return;

	}



	HWND hWndChild=NULL;

	while(NULL!=(hWndChild=FindWindowEx(hWnd,hWndChild,NULL,NULL))!=NULL)

	{

		SearchTopLevelWindow(hWndLargest,largestWndArea,hWndChild,procId,hWndExclude);

	}

}



////////////////////////////////////////////////////////////







YsSoundPlayer::APISpecificData::APISpecificData()

{

	hWndMain=nullptr;

	hOwnWnd=nullptr;

	dSound8=nullptr;

	CleanUp();

}

YsSoundPlayer::APISpecificData::~APISpecificData()

{

	CleanUp();

}



void YsSoundPlayer::APISpecificData::CleanUp(void)

{

	hWndMain=nullptr;

	if(nullptr!=dSound8)

	{

		dSound8->Release();

		dSound8=nullptr;

	}



	DestroyDummyWindow();

}



void YsSoundPlayer::APISpecificData::Start(void)

{

	CleanUp();



	MainWindowFinder mainWindowFinder;

	hWndMain=mainWindowFinder.Find();

	if(nullptr==hWndMain && nullptr==hOwnWnd)

	{

		OpenDummyWindow();

	}

	if(DS_OK==DirectSoundCreate8(NULL,&dSound8,NULL))

	{

		if(nullptr!=hWndMain)

		{

			dSound8->SetCooperativeLevel(hWndMain,DSSCL_PRIORITY);

		}

		else if(nullptr!=hOwnWnd)

		{

			dSound8->SetCooperativeLevel(hOwnWnd,DSSCL_PRIORITY);

		}

	}

}

void YsSoundPlayer::APISpecificData::End(void)

{

	CleanUp();

}



void YsSoundPlayer::APISpecificData::RefetchMainWindowHandle(void)

{

	if(nullptr!=hOwnWnd && nullptr==hWndMain)

	{

		MainWindowFinder mainWindowFinder;

		hWndMain=mainWindowFinder.Find(hOwnWnd);

		if(nullptr!=hWndMain)

		{

			dSound8->SetCooperativeLevel(hWndMain,DSSCL_PRIORITY);

			DestroyDummyWindow();

		}

	}

}



////////////////////////////////////////////////////////////



YsSoundPlayer::APISpecificData *YsSoundPlayer::CreateAPISpecificData(void)

{

	return new APISpecificData;

}

void YsSoundPlayer::DeleteAPISpecificData(APISpecificData *ptr)

{

	delete ptr;

}



YSRESULT YsSoundPlayer::StartAPISpecific(void)

{

	api->Start();

	return YSOK;

}

YSRESULT YsSoundPlayer::EndAPISpecific(void)

{

	api->End();

	return YSOK;

}



void YsSoundPlayer::SetVolumeAPISpecific(SoundData &dat,float vol)

{

	if(nullptr!=dat.api->dSoundBuf)

	{

		vol=sqrt(vol);



		float dB=(float)DSBVOLUME_MAX*vol+(float)DSBVOLUME_MIN*(1.0-vol);

		long atten=(long)dB;

		if(DSBVOLUME_MAX<atten)

		{

			atten=DSBVOLUME_MAX;

		}

		if(atten<DSBVOLUME_MIN)

		{

			atten=DSBVOLUME_MIN;

		}

		// printf("%d\n",atten);

		dat.api->dSoundBuf->SetVolume(atten);

	}

}



YSRESULT YsSoundPlayer::PlayOneShotAPISpecific(SoundData &dat)

{

	if(nullptr==api->hWndMain && nullptr!=api->hOwnWnd)

	{

		api->RefetchMainWindowHandle();

	}



	if(nullptr!=dat.api->dSoundBuf)

	{

		SetVolumeAPISpecific(dat,dat.playBackVolume);

		dat.api->dSoundBuf->SetCurrentPosition(0);

		dat.api->dSoundBuf->Play(0,0xc0000000,0);

		return YSOK;

	}

	return YSERR;

}



YSRESULT YsSoundPlayer::PlayBackgroundAPISpecific(SoundData &dat)

{

	if(nullptr==api->hWndMain && nullptr!=api->hOwnWnd)

	{

		api->RefetchMainWindowHandle();

	}



	if(nullptr!=dat.api->dSoundBuf)

	{

		DWORD sta;

		dat.api->dSoundBuf->GetStatus(&sta);

		if(0==(sta&DSBSTATUS_PLAYING))

		{

			SetVolumeAPISpecific(dat,dat.playBackVolume);

			dat.api->dSoundBuf->SetCurrentPosition(0);

			dat.api->dSoundBuf->Play(0,0xc0000000,DSBPLAY_LOOPING);

		}

		return YSOK;

	}

	return YSERR;

}



YSBOOL YsSoundPlayer::IsPlayingAPISpecific(const SoundData &dat) const

{

	if(nullptr!=dat.api->dSoundBuf)

	{

		DWORD sta;

		dat.api->dSoundBuf->GetStatus(&sta);

		if(0!=(sta&DSBSTATUS_PLAYING))

		{

			return YSTRUE;

		}

	}

	return YSFALSE;

}



void YsSoundPlayer::PauseAPISpecific(SoundData &dat)

{

	if(nullptr!=dat.api->dSoundBuf)

	{

		dat.api->dSoundBuf->Stop();

	}

}

void YsSoundPlayer::ResumeAPISpecific(SoundData &dat)

{

	if(nullptr==api->hWndMain && nullptr!=api->hOwnWnd)

	{

		api->RefetchMainWindowHandle();

	}

	if(nullptr!=dat.api->dSoundBuf)

	{

		dat.api->dSoundBuf->Play(0,0xc0000000,0);

	}

}



double YsSoundPlayer::GetCurrentPositionAPISpecific(const SoundData &dat) const

{

	DWORD playCursor,writeCursor;

	if(nullptr!=dat.api->dSoundBuf && DS_OK==dat.api->dSoundBuf->GetCurrentPosition(&playCursor,&writeCursor))

	{

		playCursor/=(dat.BytePerSample()*dat.GetNumChannel());

		return ((double)playCursor)/(double)dat.PlayBackRate();

	}

	return 0.0;

}



void YsSoundPlayer::StopAPISpecific(SoundData &dat)

{

	if(nullptr!=dat.api->dSoundBuf)

	{

		dat.api->dSoundBuf->Stop();

		// Memo: IDirectSoundBuffer::Stop() actually pauses playing.

		//       It does not rewind.  Therefore, to play from the head of the wave,

		//       IDirectSoundBuffer::SetCurrentPosition(0) must be called before playing.

	}

}



void YsSoundPlayer::KeepPlayingAPISpecific(void)

{

}



////////////////////////////////////////////////////////////



YsSoundPlayer::SoundData::APISpecificDataPerSoundData::APISpecificDataPerSoundData()

{

	dSoundBuf=nullptr;

	CleanUp();

}

YsSoundPlayer::SoundData::APISpecificDataPerSoundData::~APISpecificDataPerSoundData()

{

	CleanUp();

}

void YsSoundPlayer::SoundData::APISpecificDataPerSoundData::CleanUp(void)

{

	if(nullptr!=dSoundBuf)

	{

		dSoundBuf->Release();

		dSoundBuf=nullptr;

	}

}



void YsSoundPlayer::SoundData::APISpecificDataPerSoundData::CreateBuffer(LPDIRECTSOUND8 dSound8,SoundData &dat)

{

	CleanUp();



	const int nChannels=dat.GetNumChannel();

	const int nBlockAlign=nChannels*dat.BitPerSample()/8;

	const int nAvgBytesPerSec=dat.PlayBackRate()*nBlockAlign;



	WAVEFORMATEX fmt;

	fmt.cbSize=sizeof(fmt);

	fmt.wFormatTag=WAVE_FORMAT_PCM;



	fmt.nChannels=nChannels;

	fmt.nSamplesPerSec=dat.PlayBackRate();

	fmt.wBitsPerSample=dat.BitPerSample();



	fmt.nBlockAlign=nBlockAlign;

	fmt.nAvgBytesPerSec=nAvgBytesPerSec;





	DSBUFFERDESC desc;

	desc.dwSize=sizeof(desc);

	// Finally!  I found it!

	// https://stackoverflow.com/questions/25829935/play-background-music-with-directsound

	// I can play sound when the window loses focus!

	desc.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_LOCDEFER|DSBCAPS_GLOBALFOCUS;

	desc.dwBufferBytes=dat.SizeInByte();

	if(DSBSIZE_MAX<desc.dwBufferBytes)

	{

		desc.dwBufferBytes=DSBSIZE_MAX;

	}

	desc.dwReserved=0;

	desc.lpwfxFormat=&fmt;

	desc.guid3DAlgorithm=GUID_NULL;

	if(DS_OK==dSound8->CreateSoundBuffer(&desc,&dSoundBuf,NULL))

	{

		auto datPtr=dat.DataPointer();



		DWORD writeBufSize1,writeBufSize2;

		unsigned char *writeBuf1,*writeBuf2;

		if(dSoundBuf->Lock(0,0,(LPVOID *)&writeBuf1,&writeBufSize1,(LPVOID *)&writeBuf2,&writeBufSize2,DSBLOCK_ENTIREBUFFER)==DS_OK &&

		   NULL!=writeBuf1)

		{

			// printf("Buffer Locked\n");



			for(int i=0; i<(int)dat.SizeInByte() && i<(int)writeBufSize1; i++)

			{

				writeBuf1[i]=datPtr[i];

			}



			dSoundBuf->Unlock(writeBuf1,writeBufSize1,writeBuf2,writeBufSize2);

		}

		else

		{

			printf("Failed to Lock Buffer.\n");

			CleanUp();

		}

	}

	

}



////////////////////////////////////////////////////////////



YsSoundPlayer::SoundData::APISpecificDataPerSoundData *YsSoundPlayer::SoundData::CreateAPISpecificData(void)

{

	return new APISpecificDataPerSoundData;

}

void YsSoundPlayer::SoundData::DeleteAPISpecificData(APISpecificDataPerSoundData *ptr)

{

	delete ptr;

}



YSRESULT YsSoundPlayer::SoundData::PreparePlay(YsSoundPlayer &player)

{

	if(nullptr!=api->dSoundBuf)

	{

		return YSOK;

	}

	if(nullptr==player.api->dSound8)

	{

		return YSERR;

	}



	api->CreateBuffer(player.api->dSound8,*this);

	if(nullptr!=api->dSoundBuf)

	{

		return YSOK;

	}

	return YSERR;

}



void YsSoundPlayer::SoundData::CleanUpAPISpecific(void)

{

	if(nullptr!=playerStatePtr && YsSoundPlayer::STATE_ENDED==*playerStatePtr)

	{

		// In this case, DirectSoundBuffer is gone with the player.

		api->dSoundBuf=nullptr;

	}

	api->CleanUp();

}







////////////////////////////////////////////////////////////



void YsSoundPlayer::APISpecificData::OpenDummyWindow(void)

{

	#define WINSTYLE WS_OVERLAPPED|WS_CAPTION|WS_VISIBLE|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN

	#define WINCLASS L"YsSimpleSound_DummyWindowClass"

	HINSTANCE inst=GetModuleHandleA(NULL);



	WNDCLASSW wc;

	wc.style=CS_OWNDC|CS_BYTEALIGNWINDOW;

	wc.lpfnWndProc=(WNDPROC)OwnWindowFunc;

	wc.cbClsExtra=0;

	wc.cbWndExtra=0;

	wc.hInstance=(HINSTANCE)inst;

	wc.hIcon=nullptr;

	wc.hCursor=nullptr;

	wc.hbrBackground=nullptr;

	wc.lpszMenuName=NULL;

	wc.lpszClassName=WINCLASS;



	if(0!=RegisterClassW(&wc))

	{

		RECT rc;

		rc.left  =0;

		rc.top   =0;

		rc.right =127;

		rc.bottom=127;

		AdjustWindowRect(&rc,WINSTYLE,FALSE);

		int wid  =rc.right-rc.left+1;

		int hei  =rc.bottom-rc.top+1;



		const wchar_t *WINTITLE=L"DummyWindowForFailedAPIDesignOfDirectSound";

		hOwnWnd=CreateWindowW(WINCLASS,WINTITLE,WINSTYLE,0,0,wid,hei,NULL,NULL,inst,NULL);



		ShowWindow(hOwnWnd,SW_SHOWNORMAL);

		SetForegroundWindow(hOwnWnd);

	}

}



void YsSoundPlayer::APISpecificData::DestroyDummyWindow(void)

{

	if(nullptr!=hOwnWnd)

	{

		DestroyWindow(hOwnWnd);

	}

	hOwnWnd=nullptr;

}



LRESULT WINAPI YsSoundPlayer::APISpecificData::OwnWindowFunc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)

{

	return DefWindowProc(hWnd,msg,wp,lp);

}



////////////////////////////////////////////////////////////



enum

{

	RINGBUFFER_MILLI=1000,



	RINGBUFFER_CHANNELS=2,

	RINGBUFFER_SAMPLING_RATE=44100,

	RINGBUFFER_LENGTH_MILLISEC=10000,

	RINGBUFFER_BITS_PER_SAMPLE=16,

};



class YsSoundPlayer::Stream::APISpecificData

{

public:

	LPDIRECTSOUNDBUFFER dSoundBuf=nullptr;



	uint64_t bufferLengthInBytes;

	int64_t seg0Start,seg0End;

	int64_t seg1Start,seg1End;

};



YsSoundPlayer::Stream::APISpecificData *YsSoundPlayer::Stream::CreateAPISpecificData(void)

{

	YsSoundPlayer::Stream::APISpecificData *api=new YsSoundPlayer::Stream::APISpecificData;

	api->dSoundBuf=nullptr;

	return api;

}

void YsSoundPlayer::Stream::DeleteAPISpecificData(APISpecificData *api)

{

	if(nullptr!=api)

	{

		if(nullptr!=api->dSoundBuf)

		{

		}

		delete api;

	}

}



YSRESULT YsSoundPlayer::StartStreamingAPISpecific(Stream &stream)

{

	auto api=stream.api;

	if(nullptr!=api)

	{

		const int nChannels=RINGBUFFER_CHANNELS;

		const int nBlockAlign=nChannels*RINGBUFFER_BITS_PER_SAMPLE/8;

		const int nAvgBytesPerSec=RINGBUFFER_SAMPLING_RATE*nBlockAlign;



		uint64_t sizeInBytes=RINGBUFFER_SAMPLING_RATE;

		sizeInBytes*=RINGBUFFER_BITS_PER_SAMPLE/8;

		sizeInBytes*=RINGBUFFER_CHANNELS;

		sizeInBytes*=RINGBUFFER_LENGTH_MILLISEC;

		sizeInBytes/=RINGBUFFER_MILLI;



		stream.api->bufferLengthInBytes=sizeInBytes;



		WAVEFORMATEX fmt;

		fmt.cbSize=sizeof(fmt);

		fmt.wFormatTag=WAVE_FORMAT_PCM;



		fmt.nChannels=nChannels;

		fmt.nSamplesPerSec=RINGBUFFER_SAMPLING_RATE;

		fmt.wBitsPerSample=RINGBUFFER_BITS_PER_SAMPLE;



		fmt.nBlockAlign=nBlockAlign;

		fmt.nAvgBytesPerSec=nAvgBytesPerSec;





		DSBUFFERDESC desc;

		desc.dwSize=sizeof(desc);

		// Finally!  I found it!

		// https://stackoverflow.com/questions/25829935/play-background-music-with-directsound

		// I can play sound when the window loses focus!

		desc.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_LOCDEFER|DSBCAPS_GLOBALFOCUS|DSBCAPS_GETCURRENTPOSITION2;

		desc.dwBufferBytes=sizeInBytes;

		if(DSBSIZE_MAX<desc.dwBufferBytes)

		{

			desc.dwBufferBytes=DSBSIZE_MAX;

		}

		desc.dwReserved=0;

		desc.lpwfxFormat=&fmt;

		desc.guid3DAlgorithm=GUID_NULL;

		if(DS_OK==this->api->dSound8->CreateSoundBuffer(&desc,&stream.api->dSoundBuf,NULL))

		{

			DWORD writeBufSize1,writeBufSize2;

			unsigned char *writeBuf1,*writeBuf2;

			if(stream.api->dSoundBuf->Lock(0,0,(LPVOID *)&writeBuf1,&writeBufSize1,(LPVOID *)&writeBuf2,&writeBufSize2,DSBLOCK_ENTIREBUFFER)==DS_OK)

			{

				if(NULL!=writeBuf1)

				{

					for(int i=0; i<(int)writeBufSize1; i++)

					{

						writeBuf1[i]=0;

					}

				}

				if(NULL!=writeBuf2)

				{

					for(int i=0; i<(int)writeBufSize2; i++)

					{

						writeBuf2[i]=0;

					}

				}



				stream.api->dSoundBuf->Unlock(writeBuf1,writeBufSize1,writeBuf2,writeBufSize2);



				stream.api->dSoundBuf->SetCurrentPosition(0);

				stream.api->dSoundBuf->Play(0,0xc0000000,DSBPLAY_LOOPING);



				stream.api->seg0Start=0;

				stream.api->seg0End=0;

				stream.api->seg1Start=0;

				stream.api->seg1End=0;

			}

			else

			{

				printf("Failed to Lock Buffer.\n");

				return YSERR;

			}

		}

		return YSOK;

	}

	return YSERR;

}

void YsSoundPlayer::StopStreamingAPISpecific(Stream &stream)

{

	if(nullptr!=stream.api && nullptr!=stream.api->dSoundBuf)

	{

		DWORD writeBufSize1,writeBufSize2;

		unsigned char *writeBuf1,*writeBuf2;



		stream.api->dSoundBuf->Stop();

		if(stream.api->dSoundBuf->Lock(0,0,(LPVOID *)&writeBuf1,&writeBufSize1,(LPVOID *)&writeBuf2,&writeBufSize2,DSBLOCK_ENTIREBUFFER)==DS_OK)

		{

			if(NULL!=writeBuf1)

			{

				for(int i=0; i<(int)writeBufSize1; i++)

				{

					writeBuf1[i]=0;

				}

			}

			if(NULL!=writeBuf2)

			{

				for(int i=0; i<(int)writeBufSize2; i++)

				{

					writeBuf2[i]=0;

				}

			}



			stream.api->dSoundBuf->Unlock(writeBuf1,writeBufSize1,writeBuf2,writeBufSize2);



			stream.api->dSoundBuf->SetCurrentPosition(0);



			stream.api->seg0Start=0;

			stream.api->seg0End=0;

			stream.api->seg1Start=0;

			stream.api->seg1End=0;

		}

	}

}

YSBOOL YsSoundPlayer::StreamPlayerReadyToAcceptNextSegmentAPISpecific(const Stream &stream,const SoundData &dat) const

{

	if(nullptr!=stream.api && nullptr!=stream.api->dSoundBuf)

	{

		DWORD playCursor,writeCursor;

		stream.api->dSoundBuf->GetCurrentPosition(&playCursor,&writeCursor);

		// Two segments: Seg0 -> Seg1.

		// Next data should be written when Seg0 is done.

		if(stream.api->seg0Start<=stream.api->seg0End)

		{

			//   0   seg0Start       seg0End

			//   |     |---------------|        |

			//                SEG0

			if(playCursor<stream.api->seg0Start || stream.api->seg0End<=playCursor)

			{

				return YSTRUE;

			}

		}

		else // means segment 0 wrapped around.

		{

			//   0   seg0End         seg0Start

			//   |----|                |--------|

			//    SEG0                    SEG0

			if(stream.api->seg0End<=playCursor && playCursor<stream.api->seg0Start)

			{

				return YSTRUE;

			}

		}

	}

	return YSFALSE;

}

YSRESULT YsSoundPlayer::AddNextStreamingSegmentAPISpecific(Stream &stream,const SoundData &dat)

{

	// How can I tell both segments lapsed?

	// If playing segment1, add it to the end of segment 1, and erase segment 0.

	// If not, lapsed or it is the first segment.  Just write it to writeCursor.

	if(nullptr!=stream.api && nullptr!=stream.api->dSoundBuf)

	{

		int64_t numSamplesIn=dat.GetNumSamplePerChannel();

		int64_t numSamplesOut=numSamplesIn;

		numSamplesOut*=RINGBUFFER_SAMPLING_RATE;

		numSamplesOut/=dat.PlayBackRate();







		DWORD playCursor,writeCursor;

		stream.api->dSoundBuf->GetCurrentPosition(&playCursor,&writeCursor);



		bool isPlayingSegment1=false;

		if(stream.api->seg1Start<=stream.api->seg1End)

		{

			//   0   seg1Start       seg1End

			//   |     |---------------|        |

			//                SEG1

			isPlayingSegment1=(stream.api->seg1Start<=playCursor && playCursor<stream.api->seg1End);

		}

		else // means segment 0 wrapped around.

		{

			//   0   seg1End         seg1Start

			//   |----|                |--------|

			//    SEG1                    SEG1

			isPlayingSegment1=(playCursor<stream.api->seg1End || stream.api->seg1Start<=playCursor);

		}



		DWORD bytesCanWrite=0;

		if(playCursor<writeCursor)

		{

			//   0   playCursor     writeCursor

			//   |    |----------------|        |bufferLength

			bytesCanWrite=stream.api->bufferLengthInBytes-(writeCursor-playCursor);

		}

		else

		{

			//   0   writeCursor     playCursor

			//   |----|                |--------|

			bytesCanWrite=playCursor-writeCursor;

		}



		if(true==isPlayingSegment1)

		{

			writeCursor=(DWORD)stream.api->seg1End;

		}



		DWORD bytesWrite=std::min<DWORD>(numSamplesOut*2*RINGBUFFER_CHANNELS,bytesCanWrite);





		DWORD writeBufSize1,writeBufSize2;

		unsigned char *writeBuf1,*writeBuf2;

		if(stream.api->dSoundBuf->Lock(writeCursor,bytesWrite,(LPVOID *)&writeBuf1,&writeBufSize1,(LPVOID *)&writeBuf2,&writeBufSize2,0)==DS_OK)

		{

			const unsigned char *readPtr=dat.DataPointer();



			int64_t balance=RINGBUFFER_SAMPLING_RATE;

			if(NULL!=writeBuf1)

			{

				unsigned char *writePtr=(unsigned char *)writeBuf1;

				uint64_t sizeLeft=writeBufSize1;

				while(0<sizeLeft)

				{

					if(1==dat.GetNumChannel())

					{

						*writePtr++=readPtr[0];

						*writePtr++=readPtr[1];

						*writePtr++=readPtr[0];

						*writePtr++=readPtr[1];

					}

					else

					{

						*writePtr++=readPtr[0];

						*writePtr++=readPtr[1];

						*writePtr++=readPtr[2];

						*writePtr++=readPtr[3];

					}



					sizeLeft-=4;



					balance-=dat.PlayBackRate();

					while(balance<0)

					{

						balance+=RINGBUFFER_SAMPLING_RATE;

						readPtr+=2*dat.GetNumChannel();

					}

				}

			}



			if(NULL!=writeBuf2)

			{

				unsigned char *writePtr=(unsigned char *)writeBuf2;

				uint64_t sizeLeft=writeBufSize2;

				while(0<sizeLeft)

				{

					if(1==dat.GetNumChannel())

					{

						*writePtr++=readPtr[0];

						*writePtr++=readPtr[1];

						*writePtr++=readPtr[0];

						*writePtr++=readPtr[1];

					}

					else

					{

						*writePtr++=readPtr[0];

						*writePtr++=readPtr[1];

						*writePtr++=readPtr[2];

						*writePtr++=readPtr[3];

					}



					sizeLeft-=4;



					balance-=dat.PlayBackRate();

					while(balance<0)

					{

						balance+=RINGBUFFER_SAMPLING_RATE;

						readPtr+=2*dat.GetNumChannel();

					}

				}

			}

			stream.api->dSoundBuf->Unlock(writeBuf1,writeBufSize1,writeBuf2,writeBufSize2);

		}





		if(true==isPlayingSegment1)

		{

			stream.api->seg0Start=stream.api->seg1Start;

			stream.api->seg0End=stream.api->seg1End;

		}

		else

		{

			// Lapsed: Make entire rest of the buffer as segment 0.

			stream.api->seg0Start=(writeCursor+bytesWrite)%stream.api->bufferLengthInBytes;

			stream.api->seg0End=writeCursor;

		}

		stream.api->seg1Start=writeCursor;

		stream.api->seg1End=(writeCursor+bytesWrite)%stream.api->bufferLengthInBytes;

		return YSOK;

	}

	return YSERR;

}

