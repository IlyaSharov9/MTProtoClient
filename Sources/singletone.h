template <class T>
class global_singletone {
    public:
    static T& instance() {
        static T value;

        return value;
    }
};