import serial
import time

def lane_change_signal():
    # 사용자로부터 방향 입력 받기
    direction = input("Enter 'q' for right, 'e' for left, or 's' to stop: ").strip()
    
    if direction == 'q':
        return 'q'  # 오른쪽 회전 명령
    elif direction == 'e':
        return 'e'  # 왼쪽 회전 명령
    elif direction == 's':
        return 's'  # 멈춤 명령
    else:
        return 's'  # 기본값: 멈춤

def main():
    # UART 설정 (시리얼 포트 설정)
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    time.sleep(2)  # 시리얼 초기화 대기 시간

    while True:
        # 방향 신호 입력받기
        direction = lane_change_signal()

        # 's'가 입력되면 프로그램 종료
        if direction == 's':
            print("Stopping...")
            break
        
        # 시리얼 포트로 방향 신호 전송
        ser.write((direction + '\n').encode())
        print(f"Sent direction: {direction}")

    ser.close()  # 시리얼 포트 닫기

if __name__ == "__main__":
    main()


