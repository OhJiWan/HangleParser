#include <nan.h>
#include <locale>
#include <iostream>
#include <typeinfo>
#include <map>
#include <string>

#define W_BUUFER_MAX 200
using namespace std;
typedef map<wstring,char *> HangleMap;


class HangleDisassembler{
		
	private :
		HangleMap testMap;
		int disassembleHangle(wchar_t * hangleStr, char * &ResultBuffer);

	public :
		int insertData(wchar_t * hangleStr);
		int init(const Nan::FunctionCallbackInfo<v8::Value> & info);
		int printAll();	
		double findSimilarString(char * compareDisassembleStr,wstring & similarString);
		v8::Local<v8::Array> GetHighProbabilityValue(const Nan::FunctionCallbackInfo<v8::Value> & info);

};

HangleDisassembler hangleDisassembler;

double HangleDisassembler::findSimilarString(char * compareDisassembleStr,wstring & similarString){

	HangleMap::iterator pos;	 
	int totalLen =0;
	int compareStrLen = (int)*(compareDisassembleStr);
	int iMax=0;
	wstring returString;
	int CorrectCount=0;
	double Correctrate = 0.0;
	double MaxCorrectrate = 0.0;


   	for (pos = testMap.begin(); pos != testMap.end(); ++pos) {       

		totalLen = (int)*((pos->second));
		(totalLen < compareStrLen) ? iMax=totalLen : iMax=compareStrLen;
		Correctrate = 0.0;
		CorrectCount=0;
		for(int i=0; i < iMax ; i++){
			if(*((pos->second)+i) == *(compareDisassembleStr+i)){			
				CorrectCount++;
			}    
		}
		Correctrate = (double)CorrectCount/(double)totalLen * 100;

		if(Correctrate > MaxCorrectrate){
			MaxCorrectrate = Correctrate;
			returString = pos->first;

		}

        
    }
	similarString = returString;
	return MaxCorrectrate;
}

v8::Local<v8::Array> HangleDisassembler::GetHighProbabilityValue(const Nan::FunctionCallbackInfo<v8::Value> & info){

	v8::Local<v8::Array> returnArray;
	wchar_t hangleStr[W_BUUFER_MAX]=L"";
	int StrLen=0;	
	char * compareDisassembleStr;
	double probability;
	wstring similarString;
	v8::Isolate * iso = info.GetIsolate();


	StrLen = info[0]->ToString()->Length();
	info[0]->ToString()->Write((uint16_t *)hangleStr,0,StrLen,0);
	disassembleHangle(hangleStr,compareDisassembleStr);
	probability = findSimilarString(compareDisassembleStr,similarString);	
	const wchar_t * view = similarString.c_str();


   v8::Local<v8::Number> rNum = v8::Number::New(iso,probability);
   v8::Local<v8::String> rString = v8::String::NewFromTwoByte(iso,(uint16_t *)view,v8::NewStringType::kNormal,similarString.length()).ToLocalChecked();
   returnArray = v8::Array::New(iso,2);   
   returnArray->Set(0,rString);
   returnArray->Set(1,rNum);



	free(compareDisassembleStr);

	return returnArray;

}


int HangleDisassembler::init(const Nan::FunctionCallbackInfo<v8::Value> & info){
	
	wchar_t hangleStr[W_BUUFER_MAX]=L"";
	int strArrayInfoLen;
	int StrLen=0;
	int i;

	v8::Isolate * iso = info.GetIsolate();
	v8::Local<v8::Array> strArrayInfo = info[0].As<v8::Array>();
	strArrayInfoLen = strArrayInfo->Length();  //넘긴 문자열 배열의 길이

	for(i=0;i<strArrayInfoLen;i++){
	StrLen = strArrayInfo->Get(i)->ToString()->Length(); //배열 하나의 String 길이를 알수 있음	
	memset(hangleStr,0x00,sizeof(hangleStr));
	strArrayInfo->Get(i)->ToString()->Write((uint16_t *)hangleStr,0,StrLen,0); //2byte 형태로 변환
	insertData(hangleStr);		
	}
	
	return 1;

}




int HangleDisassembler::insertData(wchar_t * hangleStr){

	char * value;	
	disassembleHangle(hangleStr,value);
    testMap[hangleStr] = value;    
	return 1;


}

int HangleDisassembler::printAll(){

	HangleMap::iterator pos;	 

   for (pos = testMap.begin(); pos != testMap.end(); ++pos) {
        wprintf(L"key !! : [%s] ",pos->first);
			int i=0;
	while(i < (int)*(pos->second)){
		
    	 printf("%02x ",*((pos->second)+i) );  //결과 출력
    	i++;
	}
        printf("\n");
    }
    
	return 1;

}


int HangleDisassembler::disassembleHangle(wchar_t * hangleStr, char * &ResultBuffer)
{

	unsigned int    pos = 1;
	char dissResultBuffer[4096];

	unsigned int buffSize = sizeof dissResultBuffer;

	while (*hangleStr != '\0')
	{
		if (*hangleStr < 256)
		{
			if (pos + 2 >= buffSize - 1)
				break;

			dissResultBuffer[pos] = (char)*hangleStr;
			++pos;
		}
		else
		{
			if (pos + 4 >= buffSize - 1)
				break;
     		dissResultBuffer[pos] = (*hangleStr - 0xAC00) / (21 * 28);
			dissResultBuffer[pos + 1] = (*hangleStr - 0xAC00) % (21 * 28) / 28;
			dissResultBuffer[pos + 2] = (*hangleStr - 0xAC00) % 28;
			pos += 3;
		}

		++hangleStr;
	}


	dissResultBuffer[0] = pos; // 배열의 사이즈를 0번지에 항상 넣는다.
	ResultBuffer  = (char *)malloc(sizeof(char)*(pos));			
	memcpy(ResultBuffer,dissResultBuffer,(pos));
	return pos;
  
}



void GetHighProbabilityValue(const Nan::FunctionCallbackInfo<v8::Value> & info) {

	v8::Local<v8::Array> returnArray = hangleDisassembler.GetHighProbabilityValue(info);
	info.GetReturnValue().Set(returnArray);
	

}




void InitHangleMap(const Nan::FunctionCallbackInfo<v8::Value> & info) {
	hangleDisassembler.init(info);
}





void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("init").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(InitHangleMap)->GetFunction());


	exports->Set(Nan::New("getHighProbabilityValue").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetHighProbabilityValue)->GetFunction());

}

NODE_MODULE(hello, Init)



void test(const Nan::FunctionCallbackInfo<v8::Value> & info) {

	wchar_t *teststr = L"한글";
	//char *  resultBuffer2;
	wchar_t hangleStr[200]=L"";

	printf("arg length : %d",info.Length());

	v8::Isolate * iso = info.GetIsolate();
	v8::Local<v8::Array> testInfo = info[0].As<v8::Array>();
	printf("array length : %d\n",testInfo->Length());  //넘긴 배열의 길이를 알 수 있음
	int StrLen = testInfo->Get(0)->ToString()->Length(); //배열 하나의 String 길이를 알수 있음	
	printf("StrLength : %d\n",StrLen);
	testInfo->Get(0)->ToString()->Write((uint16_t *)hangleStr,0,StrLen,0); //2byte 형태로 변환
	// int pos = disassembleHangle(hangleStr, resultBuffer2); //분석 하고
	
	// int i=0;
	// while(i < pos){
    // 	 printf("%02x ",resultBuffer2[i]);  //결과 출력
    // 	i++;
	// }
	// printf("\n");

   //v8::Local<v8::Number> num = Nan::New(8787); //이건 숫자 반환 연습 해봤음. // 이제 남은것은 어떻게 객체를 반환하느냐? 

   v8::Local<v8::Number> num = testInfo->Get(1)->ToNumber();
   v8::Local<v8::Number> tNum = v8::Number::New(iso,89.5);
	wchar_t *teststr2 = L"사랑해요";
   //v8::Local<v8::String> tString = v8::String::NewFromTwoByte(iso,(uint16_t *)teststr2,v8::NewStringType::kNormal,wcslen(teststr2)).ToLocalChecked();
   v8::Local<v8::String> tString = v8::String::NewFromTwoByte(iso,(uint16_t *)hangleStr,v8::NewStringType::kNormal,wcslen(hangleStr)).ToLocalChecked();
   v8::Local<v8::Array> returnArray = v8::Array::New(iso,2);
   
   returnArray->Set(0,tString);
   returnArray->Set(1,tNum);
   // = Nan::New<T>(tNum);
   info.GetReturnValue().Set(returnArray);

}
