source ./core.sh

current_symbol="X"
other_symbol="O"

main() {
    while true; do
        draw_state
        draw_prompt
        change_state $number $current_symbol
        send_turn
        draw_state
        if check_win $current_symbol; then
            echo "You won"
            break
        fi
        if check_draw; then
            echo "Draw"
            break
        fi
        listen
        change_state $coord $other_symbol
        draw_state
        if check_win $other_symbol; then
            echo "You lose"
            break
        fi
        if check_draw; then
            echo "Draw"
            break
        fi
        done
}

main