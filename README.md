# 비슷한 문자 찾아내는 Node.js C++ 모듈


● 기능 설명 및 사용 방법 .
:

1. 우선 컴파일을 다하고 나온 .node 파일을 require한다.
    const HGmodule = require('./build/Release/sentenceComparatorModule.node');

2. 처음에 한글이든 영어이든 문자열 배열을 init함수에 넣어 초기화 시킨다.

    var commandlist = ["수고했어", "사용가능한질문", "조용히해", "새로고침",
    "화면켜", "화면꺼", "알람꺼", "소리켜", "소리줄여"]
    
    HGmodule.init(commandlist);
    
3. 찾고자 하는 문자열을 입력한다. 그리고 리턴 받을 변수 하나를 선언하여, 리턴값을 받는다.

    var test = HGmodule.getHighProbabilityValue("사과했어");

4. 결과를 출력한다. 0번지에는 가장 비슷한 문자열, 1번지에는 비슷한 확율이 들어있다.

    console.log(test[0]);
    console.log(test[1]);
    
5. 출력 값

    수고했어
    84.61538461538461





● 사용전에 설치해야되는 Node.js 모듈 및 준비 사항.
:
  
  설치 모듈 :
  1. bindings
  2. nan

  실행 js 파일은 utf8로 인코딩해야 된다.



● 개발 하게된 계기 및 간단한 동작 원리.
:

스마트미러 개발중, 음성 명령어를 크롬 브라우저에서 제공하는 webkitSpeechRecognition를 통해 인식했다. webkitSpeechRecognition는 "안녕하세요"라고 말하면, 그것을 해석해서Text로 반환해주는 모듈인데. 저 자식이 문제가 있었다.
"안녕하세요"라고 말하면, 가끔 "양양하세요"라고 Text를 반환해주고, "침실"이라고 말하면 "잠실"이라고 Text를 반환해주는 것이다. 그래서 계속 말을 반복해야 되는 문제가 있었다.

인공지능이 하나도 안 똑똑해 보여서 똑똑해 보이기 위해서 위 모듈을 만들었다.
우선 음성인식이 필요한 명령어를 한번에 입력해놓고, 전부 초성 중성 종성으로 나눈 후, 잘못된 명령어가 들어오면.
잘못된 명령어도 초성 중성 종성으로 나눠서, 이미 입력된 명령어와 비교를 한다.
비교 후 가장 비슷한 명령어를 찾아서 몇 프로가 같은지를 알려준다.



● 그외 하고 싶은 말 .
:

간단히 생각해서 구현해봤는데, 한달정도 걸렸다.
C++ 문법도 다까먹어서 다시한번 봤고, v8엔진 및 C++ 모듈 만드는 법을 다 공부 해야 했다.
어찌 어찌 구현이 됐는데, 상당히 빠르고 효과가 좋다.
진짜 잘 찾아낸다. 상상한 그대로 동작하니 기쁘다.

다만 이건 실험을 위해 만든거고 상품이 아니기 때문에 다소 코드에 헛점들이 있고, 위험한 부분이 있다.
솔직히 이 모듈이 누가 필요하겠냐만은 그냥 올려 놨다.
그리고 영어로 작성하지 않은 이유는 당연히 외국인이 이 모듈을 쓸 이유가 없다고 판단했기 때문이다.
그리고 정말 결정적으로 나는 영어 작문을 못한다.
I can't speak and write english!!


