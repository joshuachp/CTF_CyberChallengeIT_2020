CC=gcc
CFLAGS=-fno-stack-protector
ODIR=build
OBJ1=start
OBJ2=crackme
DISK_NAME=disk
ARC_NAME=sneaky.tar.gz

make: create_folders create_disk compile_start compile_crackme compress_files mount_disk copy_files remove_files umount_disk copy_to_site
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
# Converto il file in un fylesystem ext2
	mkfs -t ext2 $(ODIR)/$(DISK_NAME)


compile_start: src/start.c
	$(CC) -o $(ODIR)/binary/$(OBJ1) $(CFLAGS) $^

compile_crackme: src/easy_password.c
	$(CC) -o $(ODIR)/binary/$(OBJ2) $(CFLAGS) $^

compress_files: $(ODIR)/binary/$(OBJ2) filesystem/final_stage.txt
# Copi i file in dir
	cp $^ $(ODIR)/archive/cache
# Comprimo i file
#	zip -j $(ODIR)/archive/$(ARC_NAME) $^
	tar -czf $(ODIR)/archive/$(ARC_NAME) -C $(ODIR)/archive/cache .

copy_files: filesystem/next_stage.txt  $(ODIR)/archive/$(ARC_NAME) filesystem/helpful.png filesystem/requirements.txt ./filesystem/get_flag.pyc
# Copio i file
	sudo cp $^ $(ODIR)/mnt

remove_files: $(ODIR)/mnt/$(ARC_NAME)
# Serve un umount per scrivere i dati nel fylesystem
	sudo umount $(ODIR)/mnt 
	sudo mount $(ODIR)/$(DISK_NAME) $(ODIR)/mnt
# Rimuovo i file da nascondere
	sudo unlink $^

mount_disk: $(ODIR)/$(DISK_NAME)
	sudo mount $(ODIR)/$(DISK_NAME) $(ODIR)/mnt

umount_disk:
	sudo umount $(ODIR)/mnt 

copy_to_site:
# Copio il base64 del disco in un file sul sito con in UUID
	base64 -w 0 $(ODIR)/$(DISK_NAME) > site/dist/0ab79b02-645e-4a72-be0d-7bfbd95df5b4.txt

clean:
# Rimuovo i file di build, se fallisce può esserci la cartella ancora montata
	rm -rf $(ODIR) || (sudo umount $(ODIR)/mnt && rm -rf $(ODIR))
