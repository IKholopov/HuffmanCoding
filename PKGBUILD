pkgname=huffman-coding
pkgver=1.1
pkgrel=1
licence=('GPL')
makedepends=('cmake' 'gcc')
arch=('x86_64' 'x86')
source=(https://github.com/IKholopov/HuffmanCoding/archive/1.01.tar.gz)
md5sums=('d2dd5c119075a1c06ae41df4ec4ee6ba')

build() {
	mkdir ./build
	tar -zxvf 1.01.tar.gz && mv HuffmanCoding-1.01/* .
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
