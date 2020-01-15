#pragma once

#define DebugLog(msg) { std::wstringstream ss; ss << msg; OutputDebugStringW(ss.str().c_str()); }

template<typename string_t>
string_t dirname(string_t source)
{
	source.erase(std::find(source.rbegin(), source.rend(), '\\').base(), source.end());
	return source;
}

template<class T>
T base_name(T const& path)
{
	return path.substr(path.find_last_of('\\') + 1);
}
template<class T>

T remove_extension(T const& filename)
{
	typename T::size_type const p(filename.find_last_of('.'));
	return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}