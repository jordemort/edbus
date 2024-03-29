%define _missing_doc_files_terminate_build 0

%{!?_rel:%{expand:%%global _rel 0.enl%{?dist}}}

Summary: EFL Wrapper for DBus
Name: e_dbus
Version: 1.7.7
Release: %{_rel}
License: BSD
Group: System Environment/Libraries
URL: http://www.enlightenment.org/
Source: %{name}-%{version}.tar.gz
Packager: %{?_packager:%{_packager}}%{!?_packager:Michael Jennings <mej@eterm.org>}
Vendor: %{?_vendorinfo:%{_vendorinfo}}%{!?_vendorinfo:The Enlightenment Project (http://www.enlightenment.org/)}
Distribution: %{?_distribution:%{_distribution}}%{!?_distribution:%{_vendor}}
Obsoletes: ecore-dbus <= 0.9.9.040
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
e_dbus provides a convenience wrapper for EFL applications using DBus.

%package devel
Summary: e_dbus headers, static libraries, documentation and test programs
Group: System Environment/Libraries
Requires: %{name} = %{version}

%description devel
Headers, static libraries, test programs and documentation for e_dbus

%prep
%setup -q

%build
%{configure} --prefix=%{_prefix}
%{__make} %{?_smp_mflags} %{?mflags}

%install
%{__make} %{?mflags_install} DESTDIR=$RPM_BUILD_ROOT install

# Get rid of unneeded testing cruft.
%{__rm} -rf $RPM_BUILD_ROOT%{_datadir}/%{name}

%clean
test "x$RPM_BUILD_ROOT" != "x/" && rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(-, root, root)
%doc AUTHORS COPYING* README
%{_bindir}/*
%{_libdir}/*.so.*

%files devel
%defattr(-, root, root)
%{_includedir}/e_dbus-1/*.h
%{_includedir}/e_dbus-1/*/*.h
%{_libdir}/*.so
%{_libdir}/*.la
%{_libdir}/*.a
%{_libdir}/pkgconfig/*

%changelog
