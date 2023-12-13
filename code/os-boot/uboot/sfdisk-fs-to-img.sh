#!/usr/bin/env bash

partition_file_1="$1"
img_file="${partition_file_1}.img"
block_size=512
partition_size_1="$(wc -c "$partition_file_1" | awk '{print $1}')"
part_table_offset=$((2**20))
cur_offset=0
bs=1024

dd if=/dev/zero of="$img_file" bs="$bs" count=$((($part_table_offset + $partition_size_1)/$bs)) skip="$(($cur_offset/$bs))"

printf "
type=83, size=$(($partition_size_1/$block_size))
" | sfdisk "$img_file"

cur_offset=$(($cur_offset + $part_table_offset))
dd if="$partition_file_1" of="$img_file" bs="$bs" seek="$(($cur_offset/$bs))"
cur_offset=$(($cur_offset + $partition_size_1))
