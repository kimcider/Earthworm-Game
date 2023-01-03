#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    free_memory(); /* restart할 수 있기에 게임 시작 전에 할당된 메모리들에 대한 free작업 수행*/
    
    stop = 0;   /* 게임진행 */
    net_mode = 0;   /* net mode 비활성화 */
    
    food_counter = 1;   /* 시작시 먹이 하나 갖고 시작*/
    update_counter = 10;    /* 10번의 frame 갱신마다 먹이 생성 */
    
    speed = 2;      /* 초기 스피드 초당 2 프레임 */
    ofSetFrameRate(speed);
    
    ofSetBackgroundColor(200, 200, 200);
    ofSetBackgroundAuto(true);
    
    is_update = 0;
    direction_changed_flag = 1;
    board_init();
    earthworm_init();
    
}



//--------------------------------------------------------------

void ofApp::update(){
    if(is_update == 0){
        is_update = 1;
        
        /* 게임 stop사인이 오지 않았을 경우 게임을 진행*/
        if(stop == 0){
            /*
             update시마다 지렁이 움직임
             움직이는데 성공하면 1을 반환
             벽이나 자기 몸등에 가로막혀 움직이는데 실패하면 0을 반환
             음식을 먹고 길어진 꼬리가 머리를 가로막아도 0을 반환
             */
            int result = move_earthworm();
            
            /* 0을 반환시 게임 종료 */
            if( result == 0 ){
                stop = 1;
                /* 게임이 끝난 후 배경을 초기화하지 않고 끝난 상황을 보여준다*/
                ofSetBackgroundAuto(false);
                return;
            }
            
            /* 프래임이 10번 갱신되면 */
            if(update_counter < 10){
                update_counter++;
            }else{
                /* 음식을 생성*/
                create_food();
                update_counter = 0;
            }
        }
        is_update = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    /* 게임이 진행중일경우 draw작업 진행*/
    if(stop == 0){
        /* net mode: 키보드 n을 눌러 활성화 가능. 방의 경계들을 그려준다 */
        if(net_mode) draw_net();

        draw_empty_board();/* 빈 게임보드 그리기*/
        draw_food();    /* 먹이를 그려준다 */
        draw_earthworm();   /* 지렁이를 그려준다 */
        direction_changed_flag = 1;  /* draw후, 지렁이의 방향전환이 반영되었음을 알린다.*/
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    /* s가 눌릴 경우 게임 중단 */
    if(key == 's'){
        stop = 1;
        ofSetBackgroundAuto(false);
    }
    /* r이 눌릴 경우 게임을 새로시작.
     (s가 눌린 이후에 게임을 이어서하는 기능이 아님을 유의)
     죽은 경우 사용할 수 있다.
     */
    if(key == 'r'){
        setup();
    }
    /* n을 누를 경우, 방의 경계들을 화면에 표시해준다. */
    if(key == 'n'){
        if(net_mode == 0) {
            net_mode = 1;
        }else{
            net_mode = 0;
        }
    }
    /* 동적 할당 해제 후 종료 */
    if (key == 'q')
    {
        free_memory();
        _Exit(0);
    }
    
    
    /* 방향키로 지렁이의 방향설정 */
    /*
     이전 방향 설정이 반영된 후에만 키를 인식할 수 있도록 하였다.
     이렇게 하지 않는 부작용은 ofApp.h파일의 direction_changed_flag부분에서 확인 가능.
     */
    if(direction_changed_flag == 1){
        direction_changed_flag = 0;
        if(key == OF_KEY_LEFT){
            /* 서쪽으로 가도록 방향을 설정 */
            /* 현재 동쪽으로 진행중일 경우 서쪽방향으로 방향설정 불가 */
            /* 모든 방향키 옵션에 대해 동일하다 */
            if(direction == EAST){
                return;
            }
            direction = WEST;
        }
        if(key == OF_KEY_RIGHT){
            if(direction == WEST){
                return;
            }
            direction = EAST;
        }
        if(key == OF_KEY_UP){
            if(direction == SOUTH){
                return;
            }
            direction = NORTH;
        }
        if(key == OF_KEY_DOWN){
            if(direction == NORTH){
                return;
            }
            direction = SOUTH;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

/*-------------------init func-----------------------*/

/* 게임판 초기화 */
void ofApp::board_init(void){
    /* 전체 게임판을 BOARD_SIZE * BOARD_SIZE로 설정 */
    /* 각 방마다의 x,y좌표와 해당 방의 상태 (빈공간, 지렁이 몸통, 먹이)를 표기 */
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            board[i][j].state = EMPTY;
            board[i][j].x_pos = ROOM_SIZE * (j + 1) + ROOM_SIZE / 2;
            board[i][j].y_pos = ROOM_SIZE * (i + 1) + ROOM_SIZE / 2;
        }
    }
}

/* 지렁이 초기화 */
void ofApp::earthworm_init(void){
    /* 지렁이의 시작방향은 북쪽 */
    direction = NORTH;
    
    /* 머리 꼬리 할당 */
    head = (body)malloc(sizeof(struct body_node));
    tail = (body)malloc(sizeof(struct body_node));
    
    head -> pre_body = NULL;
    head -> belong_body = tail;
    tail -> pre_body = head;
    tail -> belong_body = NULL;
    
    head -> room_col = BOARD_SIZE / 2;
    head -> room_row = BOARD_SIZE / 2;
    tail -> room_row = head -> room_row + 1;
    tail -> room_col = head -> room_col;
    
    /* 머리와 꼬리가 위치한 방의 상태를 BODY(지렁이 몸통)으로 설정 */
    board[head->room_row][head->room_col].state = BODY;
    board[tail->room_row][tail->room_col].state = BODY;
}

/*-------------------게임 진행-----------------------*/
/*
 update시 호출
 지렁이의 꼬리를 머리로 가져와 지렁이가 움직이는 것처럼 보이게 만든다.
 꼬리가 머리에 붙는 위치는 direction변수에 따라 달라진다.
 direction방향이 빈 공간이거나 먹이여서 진행할 수 있으면, 꼬리를 머리에 붙인 후 return 1
    direction방향이 먹이인 경우, 지렁이의 몸을 늘려주는 함수 필요
 direction방향이 벽이거나 지렁이의 몸이여서 진행할 수 없으면 꼬리를 머리에 붙인 후 return 0
 */
int ofApp::move_earthworm(void){
    body temp = head;
    
    /* 본래 머리와 꼬리의 위치 기억 */
    int original_head_room_row = head -> room_row;
    int original_head_room_col = head -> room_col;
    int original_tail_room_row = tail -> room_row;
    int original_tail_room_col = tail -> room_col;
    
    /* temp를 꼬리로 이동 */
    temp = tail;
    
    /* 꼬리를 본래 꼬리 바로 앞의 몸통으로 변경 후, 꼬리자르기 */
    tail = temp -> pre_body;
    tail -> belong_body = NULL;
    
    /* 꼬리를 머리 앞으로 이동 */
    body pre_head = head;
    head = temp;
    head -> pre_body = NULL;
    head -> belong_body = pre_head;
    pre_head -> pre_body = head;
    
    /* 새로 생긴 머리의 row,col정보를 기존의 머리의 row,col정보와 direction정보를 바탕으로 변경 */
    switch(direction){
        case NORTH:
            head -> room_row = original_head_room_row - 1;
            head -> room_col = original_head_room_col;
            break;
        case SOUTH:
            head -> room_row = original_head_room_row + 1;
            head -> room_col = original_head_room_col;
            break;
        case WEST:
            head -> room_row = original_head_room_row;
            head -> room_col = original_head_room_col - 1;
            break;
        case EAST:
            head -> room_row = original_head_room_row;
            head -> room_col = original_head_room_col + 1;
            break;
    }
    
    /* 벽이나 자신의 몸통을 만난 경우 0을 반환 --> 게임 종료 */
    if(head -> room_row < 0 || head ->room_row >= BOARD_SIZE || head -> room_col < 0 || head -> room_col >= BOARD_SIZE){
        return 0;
    }else if(board[head->room_row][head->room_col].state == BODY){
        return 0;
    }
    
    /* 지렁이가 빈 방으로 이동한 경우*/
    if(board[head->room_row][head->room_col].state == EMPTY){
        /* 기존 꼬리가 있던 방 상태를 EMPTY로 변경 */
        /* 새로운 머리가 달릴 방 상태를 BODY로 변경 */
        board[head->room_row][head->room_col].state = BODY;
        board[original_tail_room_row][original_tail_room_col].state = EMPTY;
        
        return 1;
    }
    /* 지렁이가 먹이가 있는 방으로 이동한 경우  */
    else if(board[head->room_row][head->room_col].state == FOOD){
        /* 기존 꼬리가 있던 방 상태를 EMPTY로 변경 */
        /* 새로운 머리가 달릴 방 상태를 BODY로 변경 */
        board[head->room_row][head->room_col].state = BODY;
        board[original_tail_room_row][original_tail_room_col].state = EMPTY;
        /* 지렁이 성장 */
        int result = groth_earthworm();
        if (result == 0) return 0; /* 먹이를 먹고 늘어난 몸통이 머리 앞을 가리게 되면 0을 반환 --> 게임종료 */
        else return 1;
    }
    
}

/* 일정 frame 갱신때마다 지렁이 먹이 생성 */
void ofApp::create_food(void){
    /* 현재 게임보드에 있는 먹이의 수가 MAX_FOOD보다 적을 경우 먹이 생성 */
    if(food_counter <= MAX_FOOD){
        food_counter++;
        
        /* 지렁이 몸통이 없는 랜덤위치에 먹이 생성 */
        int rand_row = rand()%BOARD_SIZE;
        int rand_col = rand()%BOARD_SIZE;
        while(board[rand_row][rand_col].state != EMPTY){
            rand_row = (rand_row + 1) % BOARD_SIZE;
            rand_col = (rand_col + 1) % BOARD_SIZE;
        }
        
        /* 먹이가 생성된 방의 상태를 FOOD로 변경 */
        board[rand_row][rand_col].state = FOOD;
    }
}

/* 지렁이가 먹이를 먹을 때 마다 지렁이 성장 및 지렁이 속도 증가*/
int ofApp::groth_earthworm(void){
    //새로 자란 꼬리가 머리위치일 경우 return 0 --> 게임종료
    //아닐경우 return 1 --> 계속진행
    
    /* 꼬리와 꼬리 바로 앞의 몸통에 대한 포인터 생성 */
    body temp_tail = tail;
    body temp_before_tail = tail->pre_body;

    /* 꼬리와 꼬리 바로 앞의 몸통의 방향에 대한 수식으로, 다음 꼬리가 생성될 위치를 계산 */
    int new_row = temp_tail->room_row - (temp_before_tail->room_row - temp_tail->room_row);
    int new_col = temp_tail->room_col - (temp_before_tail->room_col - temp_tail->room_col);
    
    /* 새로 생성된 꼬리의 위치가 몸통이라면 return 0 --> 게임종료*/
    if(board[new_row][new_col].state == BODY){
        return 0;
    }else{
        food_counter--; /* 지렁이가 먹었으니 게임보드의 음식 갯수를 줄여주고 */
        
        /* 새로 생긴 몸통을 할당 및 기존의 꼬리로 만들어주기*/
        body temp = (body)malloc(sizeof(struct body_node));
        temp->room_row = new_row;
        temp->room_col = new_col;
        temp->pre_body = tail;
        temp->belong_body = NULL;
        tail -> belong_body = temp;
        tail = temp;
        
        /* 지렁이가 먹이를 먹으면 속도 상승 */
        speed+=0.3;
        /* 대략 3~4번 먹을때마다 속도가 상승하도록 설정 */
        ofSetFrameRate((int)speed);
        /* 정상적으로 성장했을 경우 return 1 --> 게임 계속 진행 */
        return 1;
    }
}

/*-------------------draw func-----------------------*/
/* 게임판 그리기 */
void ofApp::draw_empty_board(void){
    /* 벽을 검은색으로 그려준다 */
    ofSetColor(0, 0, 0);
    int x_pos = ROOM_SIZE;
    int y_pos = ROOM_SIZE;
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( j == 0 ){
                ofDrawLine(GET_POS(j),GET_POS(i),GET_POS(j),GET_POS(i+1));
            }
            if( i == 0 ){
                ofDrawLine(GET_POS(j),GET_POS(i),GET_POS(j+1),GET_POS(i));
            }
            if( i == BOARD_SIZE - 1){
                ofDrawLine(GET_POS(j),GET_POS(i+1),GET_POS(j+1),GET_POS(i+1));
            }
            if( j == BOARD_SIZE - 1){
                ofDrawLine(GET_POS(j+1), GET_POS(i), GET_POS(j+1), GET_POS(i+1));
            }
        }
    }
}

/* net mode 활성화시 방의 경계 그리기 */
void ofApp::draw_net(){
    for(int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            ofSetColor(0, 128, 0);
            ofDrawLine(board[i][j].x_pos - ROOM_SIZE / 2, board[i][j].y_pos - ROOM_SIZE / 2, board[i][j].x_pos - ROOM_SIZE / 2, board[i][j].y_pos + ROOM_SIZE / 2);
            ofDrawLine(board[i][j].x_pos - ROOM_SIZE / 2, board[i][j].y_pos - ROOM_SIZE / 2, board[i][j].x_pos + ROOM_SIZE / 2, board[i][j].y_pos - ROOM_SIZE / 2);
        }
    }
}

/* 지렁이 그리기 */
void ofApp::draw_earthworm(void){
    /*
     doubly linked list로 그려진 지렁이를 while문을 통해 head부터 하나씩 다음 몸통으로 이동해가며 몸을 그려준다.
     머리의 경우는 눈을 그려준다.
     */
    body temp = head;
    while(temp){
        /* 몸통 위치 확보 */
        int x = GET_POS(temp -> room_col + 1);
        int y = GET_POS(temp -> room_row + 1);
        
        /* 몸통 그리기 */
        ofSetColor(150,75,0);
        ofDrawRectangle(x - ROOM_SIZE, y - ROOM_SIZE, ROOM_SIZE,ROOM_SIZE);
  
        /* 머리의 경우 눈을 그려준다 */
        if(temp == head){
            ofSetColor(255, 255, 255);
            ofDrawCircle(x-ROOM_SIZE/2, y-ROOM_SIZE/2, ROOM_SIZE/2.3);
            body temp_head = head;
            body temp_next = head->belong_body;
            
            int new_row =(temp_next->room_row - temp_head->room_row);
            int new_col =(temp_next->room_col - temp_head->room_col);
            
            
            ofSetColor(0, 0, 0);
            /* 머리와 머리 다음의 몸통을 이용해 지렁이의 눈동자의 방향을 결정 */
            if(new_col == 0 && new_row == 1){//NORTH
                ofDrawCircle(x-ROOM_SIZE / 2, y-ROOM_SIZE * 2.5 / 4, ROOM_SIZE / 4);
            }
            if(new_col == 0 && new_row == -1){//SOUTH
                ofDrawCircle(x-ROOM_SIZE / 2, y-ROOM_SIZE * 1.5 / 4, ROOM_SIZE / 4);
            }
            if(new_col == -1 && new_row == 0){//EAST
                ofDrawCircle(x-ROOM_SIZE * 1.5 / 4, y-ROOM_SIZE / 2, ROOM_SIZE / 4);
                
            }
            if(new_col == 1 && new_row == 0){//WEST
                ofDrawCircle(x-ROOM_SIZE * 2.5 / 4, y-ROOM_SIZE / 2, ROOM_SIZE / 4);
            }
        }
        /* 다음 몸통으로 이동 */
        temp = temp -> belong_body;
    }
}

/* 지렁이 먹이 그리기 */
void ofApp::draw_food(void){
        /*
         방의 상태가 FOOD인 방의 중앙에 food를 그림
         */
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++){
                if(board[i][j].state == FOOD ){
                    ofSetColor(0, 0, 0);
                    ofDrawCircle(board[i][j].x_pos-ROOM_SIZE / 10, board[i][j].y_pos- ROOM_SIZE / 10 , ROOM_SIZE / 5);
                }
                
            }
        }
}
/*-------------------quit func-----------------------*/
/* 지렁이의 머리부터 꼬리까지 하나씩 따라가며 메모리 할당해제를 진행 */
void ofApp::free_memory(void){
    body temp = head;
    while(temp){
        body hold = temp;
        temp = temp -> belong_body;
        free(hold);
    }
}
