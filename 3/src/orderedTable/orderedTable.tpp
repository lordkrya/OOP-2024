namespace table {

template<std::totally_ordered Tk, typename T>
void swap(OrderedTable<Tk, T>& first, OrderedTable<Tk, T>& second) noexcept {
    std::swap(first.arr_, second.arr_);
    std::swap(first.size_, second.size_);
    std::swap(first.capacity_, second.capacity_);
    swap(first.initVec_, second.initVec_);
}

}