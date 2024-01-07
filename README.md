# Earthworm-Game
원작: Snake Game  

- 실행환경: Xcode 14.1  
	OpenFrameWork의 새 프로젝트를 만든 후, src파일을 위의 파일로 변경하여 실행 가능
  키보드가 영문/소문자인 상태에서 실행할것

- 게임 환경설정  
  ofApp.h파일 내의 변수를 변경함으로써 게임의 설정변경 가능  
  	BOARD_SIZE: 게임보드의 크기 설정. BOARD_SIZE * BOARD_SIZE 크기의 게임보드에서 지렁이게임 진행  
	  ROOM_SIZE: 한 방의 크기 설정.  
	  MAX_FOOD: 게임보드에 동시에 존재할 수 있는 먹이의 갯수 설정  

- 조작키  
  방향키 - 방향키를 누른 방향으로 지렁이가 이동  
  s - 게임 중단  
  r - 새로운 게임 시작  
  n - 방의 경계를 그려준다.  
  q - 게임 종료  
  
	![image](https://user-images.githubusercontent.com/105146508/210339079-ac97b5e7-f99a-43f2-bb44-74193207550b.png)
