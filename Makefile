# Derleyici
CC = x86_64-w64-mingw32-gcc

# Derleme bayrakları
CFLAGS = -Wall -g

# Bağlama bayrakları (ncurses kütüphanesi için)
LDFLAGS = -lncursesw

# Hedef dosya
TARGET_DIR = bin
TARGET = $(TARGET_DIR)/main

# Kaynak dosyalar
SRCS = src/main.c src/hwinfo.c src/cpu.c src/gpu.c src/storage.c src/swinfo.c src/user.c src/de.c src/system.c

# Nesne dosyaları (kaynak dosyalar ile aynı ada sahip .o dosyaları)
OBJS = $(SRCS:.c=.o)

# Varsayılan hedef: bin/main oluşturulacak
all: $(TARGET)

# Hedef: ../bin/main oluşturulacak
$(TARGET): $(OBJS)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Hedef: .o dosyalarını derle
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Hedef: Temizlik işlemi (main ve *.o dosyaları silinir, .c dosyaları korunur)
clean:
	rm -f $(OBJS) $(TARGET)
	@echo temizlendi
