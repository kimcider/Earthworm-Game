#pragma once

#include "ofMain.h"
#include <math.h>
#define BOARD_SIZE 30
#define ROOM_SIZE 20
#define MAX_FOOD 10

#define GET_POS(i) ROOM_SIZE * (i + 1)
class ofApp : public ofBaseApp{
    //게임보드, 방-------------------------------------------------------
    /* 게임보드의 각 cell의 정보를 저장 */
    typedef struct{
        int x_pos;
        int y_pos;
        int state;
    }room;
    
    /* 게임보드 */
    room board[BOARD_SIZE][BOARD_SIZE];
    
    /*
     방의 상태를 기술
     EMPTY: 빈공간
     EARTHWORM: 지렁이의 몸체가 있는 공간
     FOOD: 지렁이의 먹이가 있는공간
     */
    enum room_condition{EMPTY,BODY,FOOD};
    
    //지렁이-------------------------------------------------------
    /*
     doubly linked list로 지렁이들을 표현
     */
    struct body_node{
        int room_row;
        int room_col;
        struct body_node* pre_body;
        struct body_node* belong_body;
    };
    typedef struct body_node* body;
    
    body head;  /* 지렁이의 머리 */
    body tail;  /* 지렁이의 꼬리 */
    int direction;  /* 지렁이의 진행방향 */
    
    /* 지렁이가 향하는 방향을 기술 */
    enum direction_information{NORTH,SOUTH,EAST,WEST};
    
    //변수들-------------------------------------------------------------
    double speed;   /* 게임 진행 속도 */
    int stop;       /* 게임 진행 여부 */
    int is_update;  /* 업데이트 도중 업데이트 작업을 하지 못하도록 block */
                    /* 아마 openframework내부적으로 당연히 구현이 되어있겠지만 혹시몰라서 추가했습니다.*/
    int update_counter; /* n번 업데이트시 먹이가 나오게 하도록 하기 위해 update횟수 카운트*/
    int direction_changed_flag;/* 한번 방향을 바꾸면 한번 draw한 이후에 방향을 바꿀 수 있게함
                           이 변수가 없을 경우 빠른 시간 내에 위 아래 오른쪽을 누를 경우, 현재 direction이 오른쪽으로 되고, 한칸 진행하기도 전에 direction이 아래쪽으로 바뀌어서 자기 몸통을 만나서 게임이 바로 끝나는 일이 발생할 수 있음*/

    int net_mode;       /* n을 누르면 활성화. n을 누르면 각 방들의 경계를 확인 가능 */
    int food_counter;    /* 총 먹이량이 일정 갯수를 넘어가지 않도록 카운트*/
    
    //함수들 --------------------------------------------------------
    /** initiate function **/
    /* 게임판 초기화 */
    void board_init(void);
    /* 지렁이 초기화 */
    void earthworm_init(void);
    
    /** 게임 진행 **/
    /* 매 frame갱신때마다 지렁이 이동 */
    int move_earthworm(void);
    /* 일정 frame 갱신때마다 지렁이 먹이 생성 */
    void create_food(void);
    /* 지렁이가 먹이를 먹을 때 마다 지렁이 성장 및 지렁이 속도 증가*/
    int groth_earthworm(void);
    
    /** draw function **/
    /* 게임판 그리기 */
    void draw_empty_board(void);
    /* net mode 활성화시 방의 경계 그리기 */
    void draw_net(void);
    /* 지렁이 그리기 */
    void draw_earthworm(void);
    /* 지렁이 먹이 그리기 */
    void draw_food(void);
    
    /** quit function **/
    void free_memory(void);
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
