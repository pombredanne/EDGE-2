#include <string>

char *basedir() {
    static std::string st;
    if( st.empty() ) {
        st.resize(2000);
        st = "./";
    }
    return &st[0];
}
