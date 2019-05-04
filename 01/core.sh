STATE=('*' '*' '*' '*' '*' '*' '*' '*' '*')

draw_state() {
    tput clear
    let index=0
    for item in "${STATE[@]}"; do
        echo -n "$item"
        index=$((index+1))
        if (( $index == 3)); then
            echo
            index=$((0))
        fi
    done
}

draw_prompt() {
    tput cup 8 0
    read -n1 -p "Enter cell number: " number
    while [[ ! $number =~ [0-9] ]] || [[ ${STATE[$number]} != '*' ]]; do
        echo
        read -n1 -p "Cell is busy! Enter another cell number: " number
    done
}

change_state() {
    STATE[$1]="$2"
}

listen() {
    coord=`nc -l -p 31337 -W 1`
}

send_turn() {
    echo "$number" | nc localhost 31337
    if [[ "$?" != "0" ]]; then
        echo
        echo "Error while sending data"
        exit 1
    fi
}

is_equal() {
    local first=${STATE[$1]}
    local second=${STATE[$2]}
    local third=${STATE[$3]}
    if [[ "$first" == "$second" ]] && [[ "$first" == "$third" ]] && [[ "$first" == "$4" ]]; then
            return 0
    fi
    return 1
}

check_win() {
    for line in {0..2}; do
        let first_line_index=$((line*3))
        if is_equal $first_line_index $first_line_index+1 $first_line_index+2 $1; then
            return 0
        fi
    done

    for column in {0..2}; do
        if is_equal $column $column+3 $column+6 $1; then
            return 0
        fi
    done

    if is_equal 0 4 8 $1; then
        return 0
    fi

    if is_equal 2 4 6 $1; then
        return 0
    fi

    return 1
}

check_draw() {
    for item in "${STATE[@]}"; do
        if [[ $item == '*' ]]; then
            return 1
        fi
    done
    return 0
}