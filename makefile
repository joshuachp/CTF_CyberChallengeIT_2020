CC=gcc
CFLAGS=-m32 -fno-stack-protector
ODIR=build
OBJ=crackme
DISK_NAME=disk
ARC_NAME=sneaky.zip

make: create_folders create_disk mount compile_binary compress_files copy_files remove_files umount
	echo "Done"

create_folders:
	# Cartella per i file compressi
	mkdir -p $(ODIR)/archive/cache
	# Cartella per il binary
	mkdir -p $(ODIR)/binary
	# Cartella per montare il disco
	mkdir -p $(ODIR)/mnt

create_disk:
	# Creo file da 1MB con solo 0 bytes
	dd if=/dev/zero of=$(ODIR)/$(DISK_NAME) count=1024
	# Converto il file in un fylesystem ext3
	mkfs -t ext2 -i 1024 -b 1024 -F  $(ODIR)/$(DISK_NAME)

compile_binary: binary/password.c
	$(CC) -o $(ODIR)/binary/$(OBJ) $(CFLAGS) $^
	strip $(ODIR)/binary/$(OBJ)

compress_files: $(ODIR)/binary/$(OBJ) filesystem/archive/helpful.png
	# cp $^ $(ODIR)/archive/cache
	# tar -czf $(ODIR)/archive/$(ARC_NAME) -C  $(ODIR)/archive/cache .
	# rm -r $(ODIR)/archive/cache
	zip -j $(ODIR)/archive/$(ARC_NAME) $^

copy_files: filesystem/stage.txt $(ODIR)/archive/$(ARC_NAME)
	sudo cp $^ $(ODIR)/mnt

remove_files: $(ODIR)/mnt/$(ARC_NAME)
	# Serce un umount per scrivere i dati nel fylesystem
	sudo umount $(ODIR)/mnt 
	sudo mount $(ODIR)/$(DISK_NAME) $(ODIR)/mnt
	sudo rm $^

mount: $(ODIR)/$(DISK_NAME)
	sudo mount $(ODIR)/$(DISK_NAME) $(ODIR)/mnt

umount:
	sudo umount $(ODIR)/mnt 

clean:
	rm -rf $(ODIR) || (sudo umount $(ODIR)/mnt && rm -rf $(ODIR))
