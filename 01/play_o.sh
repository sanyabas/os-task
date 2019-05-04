source ./core.sh

current_symbol="O"
other_symbol="X"

main() {
    while true; do
        draw_state
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
        draw_prompt
        change_state $number $current_symbol
        draw_state
        send_turn
        if check_win $current_symbol; then
            echo "You won"
            break
        fi
        if check_draw; then
            echo "Draw"
            break
        fi
    done
}

main