#!/bin/bash

BOLD=$(tput bold)
NORMAL=$(tput sgr0)

RED=$(tput setaf 1)
YELLOW=$(tput setaf 3)
GREEN=$(tput setaf 2)
CYAN=$(tput setaf 6)
NOCOLOR=$(tput setaf 7)

# Update directories based on OS
CWD=$(pwd)
DEPS_DIRNAME=juniper/deps
DEPS_ABS_DIR=${CWD}/${DEPS_DIRNAME}
VULKAN_DIRNAME=vulkansdk-macos
#VULKAN_ABS_DIR=${CWD}/${VULKAN_DIRNAME}
VULKAN_ABS_DIR=${DEPS_ABS_DIR}/${VULKAN_DIRNAME}
VULKAN_VOLUME_NAME=${VULKAN_DIRNAME}-1.2.170.0
VULKAN_DMG=${VULKAN_VOLUME_NAME}.dmg
VULKAN_DL_URL=https://sdk.lunarg.com/sdk/download/1.2.170.0/mac/${VULKAN_DMG}

echo "${BOLD}${CYAN}----- ----- ----- ----- ----- -----"
echo "         Installing Vulkan         "
echo "----- ----- ----- ----- ----- -----${NORMAL}${NOCOLOR}"

# Download the vulkan sdk
echo "${BOLD}${GREEN} -- Downloading${NORMAL}${NOCOLOR} VulkanSDK to ${VULKAN_DMG}"
curl ${VULKAN_DL_URL} --output ${VULKAN_DMG}

# Mount the disk image
echo "${BOLD}${GREEN} -- Mounting   ${NORMAL}${NOCOLOR} VulkanSDK disk image"
sudo hdiutil attach ${VULKAN_DMG}

# Extract the files from the volume
echo "${BOLD}${GREEN} -- Extracting ${NORMAL}${NOCOLOR} files to ${VULKAN_ABS_DIR}"
mkdir ${VULKAN_ABS_DIR}
cp -R /Volumes/${VULKAN_VOLUME_NAME}/* ${VULKAN_ABS_DIR}/

# Unmount the disk image
echo "${BOLD}${GREEN} -- Unmounting ${NORMAL}${NOCOLOR} VulkanSDK disk image"
sudo hdiutil detach /Volumes/${VULKAN_VOLUME_NAME}

# Clean the dmg from this directory
echo "${BOLD}${GREEN} -- Removing   ${NORMAL}${NOCOLOR} VulkanSDK disk image"
rm ${VULKAN_DMG}

# CD into the vulkan directory and install to system path
#echo "${BOLD}${GREEN} -- Installing ${NORMAL}${NOCOLOR} VulkanSDK to /usr/local"
#cd ${VULKAN_DIRNAME}
#sudo ./install_vulkan.py
#cd ..

echo "${BOLD}${GREEN}Successfully Installed Vulkan${NORMAL}${NOCOLOR}"
