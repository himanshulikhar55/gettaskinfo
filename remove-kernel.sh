#!/bin/bash

folder_name=$1

# Check if the input starts with "5.15.0"
# replace 5.15.0* with the kernel series you never want to be removed
if [[ "$folder_name" == 5.15.0* ]]; then
    echo "Error: You cannot delete folders starting with '5.15.0'"
    exit 1
fi

# Correct the file paths
vmlinuz_file="/boot/vmlinuz-$1"
system_map_file="/boot/System.map-$1"
config_file="/boot/config-$1"
initrd_file="/boot/initrd.img-$1"
lib_modules_folder="/lib/modules/$1"

# Check and remove the vmlinuz file
if [ -e "$vmlinuz_file" ]; then
    sudo rm -r "$vmlinuz_file"
    echo "File '$vmlinuz_file' has been deleted."
else
    echo "File '$vmlinuz_file' does not exist."
fi

# Check and remove the System.map file
if [ -e "$system_map_file" ]; then
    sudo rm -r "$system_map_file"
    echo "File '$system_map_file' has been deleted."
else
    echo "File '$system_map_file' does not exist."
fi

# Check and remove the config file
if [ -e "$config_file" ]; then
    sudo rm -r "$config_file"
    echo "File '$config_file' has been deleted."
else
    echo "File '$config_file' does not exist."
fi

# Check and remove the initrd.img file (new addition)
if [ -e "$initrd_file" ]; then
    sudo rm -r "$initrd_file"
    echo "File '$initrd_file' has been deleted."
else
    echo "File '$initrd_file' does not exist."
fi

if [ -e "$lib_modules_folder" ]; then
    sudo rm -r "$lib_modules_folder"
    echo "File '$lib_modules_folder' has been deleted."
else
    echo "File '$lib_modules_folder' does not exist."
fi
