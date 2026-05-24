namespace table {

template<std::totally_ordered Tk, typename T, bool is_const>
void swap(OTableIterator<Tk, T, is_const>& first, OTableIterator<Tk, T, is_const>& second) noexcept {
    std::swap(first.value_, second.value_);
}

}
