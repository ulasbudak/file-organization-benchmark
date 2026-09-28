CC ?= gcc
SRC := $(wildcard src/*.c)

# macOS'ta OpenSSL genelde Homebrew ile gelir; Linux'ta sistemdedir.
OPENSSL_PREFIX := $(shell brew --prefix openssl 2>/dev/null)

ifeq ($(OPENSSL_PREFIX),)
CFLAGS := -Wall -O2
LDFLAGS := -lssl -lcrypto -lz
else
CFLAGS := -Wall -O2 -I$(OPENSSL_PREFIX)/include
LDFLAGS := -L$(OPENSSL_PREFIX)/lib -lssl -lcrypto -lz
endif

all: app

app: $(SRC)
	$(CC) $(SRC) -o app $(CFLAGS) $(LDFLAGS)

run: app
	./app

clean:
	rm -rf app app.dSYM output

.PHONY: all run clean
