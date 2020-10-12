Summary: A program that plays Othello/Reversi
Name: atilo
Version: 1
Release: 2
%define src %{name}
License: BSD
Group: Amusements/Games
URL:		https://github.com/osingla/atilo
Prefix: %{_prefix}
Buildroot: %{_tmppath}/%{name}
#BuildRequires:	pkgconfig

%description
This program allows the user to play the game of Othello/Reversi
against his computer (preferably using Linux).

%prep
#rm -rf work
#mkdir work
#cd work
#wget -E --ignore-length http://singla.us/cgi-bin/viewcvs.cgi/atilo/atilo.tar.gz?tarball=1
#tar -z -f "atilo.tar.gz?tarball=1" -x 

%build
#cd work/atilo
#unset DEBUG
#make

%install
#cd work/atilo

#%{__make} install \
#	DESTDIR=$RPM_BUILD_ROOT

%files
%attr(755,root,root) %{_bindir}/*

%clean
#rm -rf $RPM_BUILD_ROOT

%changelog
* Sun Mar 30 2003 Olivier Singla <olivier@singla.us>
- Fixed a small glitch in the Makefile that prevented to have the TAGS
generated correctly

* Sat Mar 29 2003 Olivier Singla <olivier@singla.us>
- Changes in the Makefile: generate prototypes (cproto) and tags
- created rpm
