include ../../Makefile.conf

incdir:
	@echo "Creating link to chip $(TARGET_CHIP) / $(TARGET_DEVICE) include"
	@rm -f chip
	@ln -s $(TARGET_CHIP) chip
	@make -C chip --no-print-directory incdir
        