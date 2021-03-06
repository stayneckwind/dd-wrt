#
# Take care of some paths
#

ifndef ATH_ROOT
ATH_ROOT=$(PWD)
endif

ifndef ATH_INSTALL
ATH_INSTALL=.
endif

ifndef RESETBUTTON
export RESETBUTTON=0x16
endif

ifndef FIS
export FIS=0
endif
export LEDCODE=1
ifndef LED1_PIN
export LED1_PIN=0xff
export LEDCODE=0
endif
ifndef LED2_PIN
export LED2_PIN=0xff
endif
ifndef LED3_PIN
export LED3_PIN=0xff
endif
ifndef LED4_PIN
export LED4_PIN=0xff
endif

ifndef TFTPPATH
TFTPPATH=/tftpboot
endif

export ECOS_REPOSITORY=$(PWD)/ecos/packages
PATH:=${PATH}:${ATH_ROOT}/tools/ecos_build/gnutools/mipsisa32-elf/bin:$(PWD)/ecos/tools/bin
#PATH:=${PATH}:${ATH_ROOT}/gnutools/mipsisa32-elf/bin:$(PWD)/ecos/tools/bin
REDBOOT_DIR=$(PWD)

# target name will match apXX name used to build path
ROMBUILD_DIR=$(ATH_INSTALL)/redboot/$@/rom
RAMBUILD_DIR=$(ATH_INSTALL)/redboot/$@/ram

#
# Board customization support
#   Env var (or make-line arg) used to alter board config are:
#      DRAM_MB   - changes MB of Dram
#      FLASH_MB  - changes MB of Rom/Flash
#      CLK_MHZ   - changes cpu clk to given value, in MHz
#      ENET_PHY  - marvell, realtek, icplus or kendin
#      TARGET_NAME - final image name
#

DRAMSZ_IMPORT=
ifdef DRAM_MB
DRAMSZ_IMPORT=ecosconfig import $(REDBOOT_DIR)/options/RAM/$(DRAM_MB)MB_ram;
endif

FLASHSZ_IMPORT=
ifdef FLASH_MB
FLASHSZ_IMPORT=ecosconfig import $(REDBOOT_DIR)/options/flash/$(FLASH_MB)MB_rom;
endif

CLK_IMPORT=
ifdef CLK_MHZ
CLK_IMPORT=ecosconfig import $(REDBOOT_DIR)/options/cpuclk/$(CLK_MHZ)MHz;
endif

ENET_PHY_IMPORT=
ifdef ENET_PHY
ENET_PHY_IMPORT=ecosconfig import $(REDBOOT_DIR)/options/eth/$(ENET_PHY)_phy;
endif

TARGET_NAME=

#
# Rule helpers
#

PLATFORM_ROM_START = rm -rf $(ROMBUILD_DIR); \
	mkdir -p $(ROMBUILD_DIR); \
	cd $(ROMBUILD_DIR); \
	ecosconfig new atheros_$@ redboot;

PLATFORM_RAM_START = rm -rf $(RAMBUILD_DIR); \
	mkdir -p $(RAMBUILD_DIR); \
	cd $(RAMBUILD_DIR); \
	ecosconfig new atheros_$@ redboot;

PLATFORM_BLD_CUST = $(DRAMSZ_IMPORT) \
	$(FLASHSZ_IMPORT) \
	$(CLK_IMPORT) \
	$(ENET_PHY_IMPORT) \
	ecosconfig tree;

PLATFORM_ROM_BUILD = make -C $(ROMBUILD_DIR); \
	cd $(ROMBUILD_DIR)/install/bin; \
	mipsisa32-elf-objcopy -O binary redboot.elf redboot.rom ;\
	if [ -n "${TARGET_NAME}" ]; then sudo cp redboot.rom $(TFTPPATH)/${TARGET_NAME}.rom; else sudo cp redboot.rom $(TFTPPATH)/$@.rom; fi

PLATFORM_RAM_BUILD = echo "\#define RESETBUTTON ${RESETBUTTON}" > ecos/packages/redboot/current/src/ramconfig.h; \
	echo "\#define FISTYPE ${FIS}" >> ecos/packages/redboot/current/src/ramconfig.h; \
	echo "\#define LEDCODE ${LEDCODE}" >> ecos/packages/redboot/current/src/ramconfig.h; \
	echo "\#define LED1_PIN ${LED1_PIN}" >> ecos/packages/redboot/current/src/ramconfig.h; \
	echo "\#define LED2_PIN ${LED2_PIN}" >> ecos/packages/redboot/current/src/ramconfig.h; \
	echo "\#define LED3_PIN ${LED3_PIN}" >> ecos/packages/redboot/current/src/ramconfig.h; \
	echo "\#define LED4_PIN ${LED4_PIN}" >> ecos/packages/redboot/current/src/ramconfig.h; \
	make -C $(RAMBUILD_DIR); \
	cd $(RAMBUILD_DIR)/install/bin; \
	mipsisa32-elf-objcopy -O binary redboot.elf redboot.bin; \
	if [ -n "${TARGET_NAME}" ]; then sudo cp redboot.elf $(TFTPPATH)/${TARGET_NAME}.ram; else sudo cp redboot.elf $(TFTPPATH)/$@.ram; fi; \
	cd ../../../../../; \
	cp $(RAMBUILD_DIR)/install/bin/redboot.elf boot/src; \
	echo "\#define RESETBUTTON ${RESETBUTTON}" >> boot/src/ramconfig.h; \
	make -C boot/src clean; \
	make -C boot/src PLATFORM=$@; \
	cp boot/src/vmlinuz $(RAMBUILD_DIR)/install/bin/redboot.elf; \
	objcopy -O binary $(RAMBUILD_DIR)/install/bin/redboot.elf $(RAMBUILD_DIR)/install/bin/redboot.rom;
	



pb42:	BOARD_TYPE=pb42
pb42:
	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
	echo "#define AR7100 1" >> boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/pb42/current/misc/redboot_RAM.ecm; \
	ecosconfig import $(REDBOOT_DIR)/options/eth_ar7100/adm_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/pb42/ram/install/bin/redboot.rom images/redboot_pb42_32M_8M_admtek.rom
#	cp redboot/pb42/ram/install/bin/redboot.img images/redboot_pb42_32M_8M_admtek.elf


pb44:   BOARD_TYPE=pb44
pb44:
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/pb44/current/misc/redboot_RAM.ecm; \
	ecosconfig import $(REDBOOT_DIR)/options/eth_ar7100/vsc8601_phy; \
	ecosconfig tree; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/pb44/ram/install/bin/redboot.rom images/redboot_pb44_32M_8M_vsc8601.rom
#	cp redboot/pb44/ram/install/bin/redboot.img images/redboot_pb42_32M_8M_kendin.elf


pb9x:   BOARD_TYPE=pb9x
pb9x:
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/pb9x/current/misc/redboot_RAM.ecm; \
	ecosconfig tree; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/pb9x/ram/install/bin/redboot.rom images/redboot_pb9x_32M_8M.rom
#	cp redboot/pb44/ram/install/bin/redboot.img images/redboot_pb42_32M_8M_kendin.elf


#
# The rules
#

pb32:
# ROM version
	$(PLATFORM_ROM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/pb32/current/misc/redboot_ROM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_ROM_BUILD)
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/pb32/current/misc/redboot_RAM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)

ap30:
# ROM version
	$(PLATFORM_ROM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap30/current/misc/redboot_ROM.ecm; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/fis_config_sep; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_ROM_BUILD)
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap30/current/misc/redboot_RAM.ecm; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/fis_config_sep; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)

ap48:
# RAM version
	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
	echo "#define AR5312 1" >> boot/src/ramconfig.h
	$(PLATFORM_RAM_START) \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom_ap48; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap48/current/misc/redboot_RAM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap48/ram/install/bin/redboot.rom images/redboot_ap48_32M_8M_kendin.rom
	cp redboot/ap48/ram/install/bin/redboot.img images/redboot_ap48_32M_8M_kendin.elf


	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
	echo "#define AR5312 1" >> boot/src/ramconfig.h
	$(PLATFORM_RAM_START) \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom_ap48; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap48/current/misc/redboot_RAM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap48/ram/install/bin/redboot.rom images/redboot_ap48_32M_8M_icplus.rom
	cp redboot/ap48/ram/install/bin/redboot.img images/redboot_ap48_32M_8M_icplus.elf


# RAM version
	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
	echo "#define AR5312 1" >> boot/src/ramconfig.h
	$(PLATFORM_RAM_START) \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap48/current/misc/redboot_RAM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap48/ram/install/bin/redboot.rom images/redboot_ap48_16M_4M_kendin.rom
	cp redboot/ap48/ram/install/bin/redboot.img images/redboot_ap48_16M_4M_kendin.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
	echo "#define AR5312 1" >> boot/src/ramconfig.h
	$(PLATFORM_RAM_START) \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap48/current/misc/redboot_RAM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap48/ram/install/bin/redboot.rom images/redboot_ap48_16M_4M_marvell.rom
	cp redboot/ap48/ram/install/bin/redboot.img images/redboot_ap48_16M_4M_marvell.elf

	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
	echo "#define AR5312 1" >> boot/src/ramconfig.h
	$(PLATFORM_RAM_START) \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap48/current/misc/redboot_RAM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap48/ram/install/bin/redboot.rom images/redboot_ap48_32M_4M_marvell.rom
	cp redboot/ap48/ram/install/bin/redboot.img images/redboot_ap48_32M_4M_marvell.elf


ap43:
# ROM version
	$(PLATFORM_ROM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap43/current/misc/redboot_ROM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_ROM_BUILD)
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig import $(ECOS_REPOSITORY)/hal/mips/ap43/current/misc/redboot_RAM.ecm; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)

ap51:
# ROM version (ap51 ROM version is ROMRAM)
	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_32M_8M_icplus.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_32M_8M_icplus.elf

	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_32M_8M_kendin.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_32M_8M_kendin.elf


	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_32M_8M_marvell.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_32M_8M_marvell.elf

	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/admtek_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_32M_8M_admtek.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_32M_8M_admtek.elf

# 16 mb version ( mb flash)

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_8M_icplus.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_8M_icplus.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_8M_kendin.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_8M_kendin.elf

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_ruckusrom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_8M_ruckus.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_8M_ruckus.elf


	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_8M_marvell.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_8M_marvell.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/admtek_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_8M_admtek.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_8M_admtek.elf



# 16 mb version ( 4 mb flash)

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_4M_icplus.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_4M_icplus.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_4M_kendin.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_4M_kendin.elf


	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_4M_marvell.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_4M_marvell.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/admtek_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_16M_4M_admtek.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_16M_4M_admtek.elf


	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_32M_4M_icplus.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_32M_4M_icplus.elf

	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_32M_4M_kendin.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_32M_4M_kendin.elf


	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_32M_4M_marvell.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_32M_4M_marvell.elf

	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/admtek_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap51/ram/install/bin/redboot.rom images/redboot_ap51_32M_4M_admtek.rom
	cp redboot/ap51/ram/install/bin/redboot.img images/redboot_ap51_32M_4M_admtek.elf




ap53:
# ROM version (ap53 ROM version is ROMRAM)
	$(PLATFORM_ROM_START) \
	ecosconfig import $(REDBOOT_DIR)/options/startup/startup_ROMRAM; \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/2MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_ROM_BUILD)
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_zlib; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/2MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)

ap61:
	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_16M_4M_icplus.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_16M_4M_icplus.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/admtek_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_16M_4M_admtek.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_16M_4M_admtek.elf
	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_16M_4M_kendin.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_16M_4M_kendin.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_16M_4M_marvell.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_16M_4M_marvell.elf


	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_32M_4M_icplus.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_32M_4M_icplus.elf
	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/admtek_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_32M_4M_admtek.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_32M_4M_admtek.elf
	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_32M_4M_kendin.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_32M_4M_kendin.elf

	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_32M_4M_marvell.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_32M_4M_marvell.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_16M_8M_icplus.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_16M_8M_icplus.elf
	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/admtek_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_16M_8M_admtek.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_16M_8M_admtek.elf
	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_16M_8M_kendin.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_16M_8M_kendin.elf

	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_16M_8M_marvell.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_16M_8M_marvell.elf

	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/icplus_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_32M_8M_icplus.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_32M_8M_icplus.elf
	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/admtek_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_32M_8M_admtek.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_32M_8M_admtek.elf
	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/kendin_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_32M_8M_kendin.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_32M_8M_kendin.elf

	echo "#define RAM_SIZE 0x2000000" > boot/src/ramconfig.h

# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/32MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/8MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/marvell_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	mkdir -p images
	cp redboot/ap61/ram/install/bin/redboot.rom images/redboot_ap61_32M_8M_marvell.rom
	cp redboot/ap61/ram/install/bin/redboot.img images/redboot_ap61_32M_8M_marvell.elf


ap65:
	echo "#define RAM_SIZE 0x1000000" > boot/src/ramconfig.h
# RAM version
	$(PLATFORM_RAM_START) \
	ecosconfig add flash; \
	ecosconfig add eth_drivers; \
	ecosconfig add compress_7zip; \
	ecosconfig import $(REDBOOT_DIR)/options/serial/115200_baud; \
	ecosconfig import $(REDBOOT_DIR)/options/RAM/16MB_ram; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/4MB_rom; \
	ecosconfig import $(REDBOOT_DIR)/options/flash/flash_width_32; \
	ecosconfig import $(REDBOOT_DIR)/options/eth/athrs26_phy; \
	$(PLATFORM_BLD_CUST)
	$(PLATFORM_RAM_BUILD)
	cp redboot/ap65/ram/install/bin/redboot.rom images/redboot_ap65_16M_4M.rom
	cp redboot/ap65/ram/install/bin/redboot.img images/redboot_ap65_16M_4M.elf


ap63:
	make ap61 FLASH_MB=2 TARGET_NAME=ap63

ap34:
	make ap30 FLASH_MB=4 TARGET_NAME=ap34
