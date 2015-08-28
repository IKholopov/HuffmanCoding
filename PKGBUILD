pkgname=huffman-coding
pkgver=1.0
pkgrel=1
licence=('GPL')
makedepends=('cmake' 'gcc')
arch=('x86_64' 'x86')
source=(HuffmanCodingSource.tar.gz)
md5sums=('bd958e01fc0a5f920a4b7fbf11272be5')

build() {
	mkdir ./build
	tar -zxvf HuffmanCodingSource.tar.gz
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make 
	cd "$srcdir"
}

package(){
	cd "$srcdir/build"
	make DESTDIR="$pkgdir/" install	
	cd "$srcdir"
	rm -r build
	mkdir -p "$pkgdir/usr/share/man/man1"
	cp "$srcdir/man/huffman/huffman.1" "$pkgdir/usr/share/man/man1/huffman.1"
}
