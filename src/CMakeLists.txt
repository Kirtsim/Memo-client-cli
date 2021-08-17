set(src_dir ${CMAKE_CURRENT_LIST_DIR})

add_executable(${exe} "")
add_library(${exe_lib} "")
target_sources(${exe} PRIVATE ${src_dir}/main.cpp)

target_sources(${exe_lib}
    PRIVATE
        ${src_dir}/main.cpp
        ${src_dir}/Client.cpp
        ${src_dir}/context/Resources.cpp
        ${src_dir}/controller/HomeController.cpp
        ${src_dir}/controller/MemoCreateController.cpp
        ${src_dir}/controller/SearchController.cpp
        ${src_dir}/manager/ControllerManager.cpp
        ${src_dir}/ncurses/BaseWindow.cpp
        ${src_dir}/ncurses/functions.cpp
        ${src_dir}/ncurses/keys.cpp
        ${src_dir}/ncurses/SubWindow.cpp
        ${src_dir}/ncurses/Window.cpp
        ${src_dir}/remote/ListMemosCall.cpp
        ${src_dir}/remote/AddMemoCall.cpp
        ${src_dir}/remote/MemoService.cpp
        ${src_dir}/remote/ListMemoRequest.cpp
        ${src_dir}/remote/ListMemoResponseData.cpp
        ${src_dir}/remote/AddMemoRequest.cpp
        ${src_dir}/remote/AddMemoResponseData.cpp
        ${src_dir}/remote/TagService.cpp
        ${src_dir}/remote/ListTagsRequest.cpp
        ${src_dir}/remote/AddTagRequest.cpp
        ${src_dir}/remote/ListTagsResponseData.cpp
        ${src_dir}/remote/AddTagResponseData.cpp
        ${src_dir}/remote/ListTagsCall.cpp
        ${src_dir}/remote/AddTagCall.cpp
        ${src_dir}/remote/utils/ModelUtils.cpp
        ${src_dir}/remote/factory/MemoCallFactory.cpp
        ${src_dir}/remote/factory/TagCallFactory.cpp
        ${src_dir}/utils/Structs.cpp
        ${src_dir}/utils/ModelMapper.cpp
        ${src_dir}/remote/utils/Mapping.cpp
        ${src_dir}/view/BaseComponent.cpp
        ${src_dir}/view/BaseView.cpp
        ${src_dir}/view/MemoCreateView.cpp
        ${src_dir}/view/MemoSearchView.cpp
        ${src_dir}/view/TagCreateView.cpp
        ${src_dir}/view/View.cpp
        ${src_dir}/view/home/HomeView.cpp
        ${src_dir}/view/home/MenuItem.cpp
        ${src_dir}/view/home/MenuView.cpp
        ${src_dir}/view/tools/Tools.cpp
        ${src_dir}/view/tools/StringTools.cpp
        ${src_dir}/view/tools/ViewFocusOperator.cpp
        ${src_dir}/view/widget/ListView.cpp
        ${src_dir}/view/widget/TextEditView.cpp
        ${src_dir}/view/widget/TextView.cpp
        ${src_dir}/view/widget/ButtonView.cpp
        ${src_dir}/view/widget/TagPickerView.cpp
        ${src_dir}/view/dialog/MessageDialog.cpp
        ${src_dir}/view/dialog/ConfirmDialog.cpp
        ${src_dir}/model/Memo.cpp
        ${src_dir}/model/Tag.cpp
    PUBLIC
        ${src_dir}/remote/RemoteDefs.hpp
        ${src_dir}/remote/ServiceEnums.hpp
    INTERFACE
        ${src_dir}/Client.hpp
        ${src_dir}/context/Resources.hpp
        ${src_dir}/controller/IController.hpp
        ${src_dir}/controller/BaseController.hpp
        ${src_dir}/controller/HomeController.hpp
        ${src_dir}/controller/MemoCreateController.hpp
        ${src_dir}/controller/SearchController.hpp
        ${src_dir}/manager/ControllerManager.hpp
        ${src_dir}/ncurses/IWindow.hpp
        ${src_dir}/ncurses/BaseWindow.hpp
        ${src_dir}/ncurses/SubWindow.hpp
        ${src_dir}/ncurses/Window.hpp
        ${src_dir}/ncurses/functions.hpp
        ${src_dir}/ncurses/keys.hpp
        ${src_dir}/remote/Call.hpp
        ${src_dir}/remote/BaseCall.hpp
        ${src_dir}/remote/ListMemosCall.hpp
        ${src_dir}/remote/AddMemoCall.hpp
        ${src_dir}/remote/MemoService.hpp
        ${src_dir}/remote/ServiceResponse.hpp
        ${src_dir}/remote/ListMemoRequest.hpp
        ${src_dir}/remote/ListMemoResponseData.hpp
        ${src_dir}/remote/AddMemoRequest.hpp
        ${src_dir}/remote/AddMemoResponseData.hpp
        ${src_dir}/remote/TagService.hpp
        ${src_dir}/remote/ListTagsRequest.hpp
        ${src_dir}/remote/AddTagRequest.hpp
        ${src_dir}/remote/ListTagsResponseData.hpp
        ${src_dir}/remote/AddTagResponseData.hpp
        ${src_dir}/remote/ListTagsCall.hpp
        ${src_dir}/remote/AddTagCall.hpp
        ${src_dir}/remote/utils/ModelUtils.hpp
        ${src_dir}/remote/utils/Mapping.hpp
        ${src_dir}/remote/factory/MemoCallFactory.hpp
        ${src_dir}/remote/factory/TagCallFactory.hpp
        ${src_dir}/utils/Enums.hpp
        ${src_dir}/utils/Structs.hpp
        ${src_dir}/utils/ModelMapper.hpp
        ${src_dir}/view/IComponent.hpp
        ${src_dir}/view/BaseComponent.hpp
        ${src_dir}/view/BaseView.hpp
        ${src_dir}/view/MemoCreateView.hpp
        ${src_dir}/view/MemoSearchView.hpp
        ${src_dir}/view/TagCreateView.hpp
        ${src_dir}/view/View.hpp
        ${src_dir}/view/home/HomeView.hpp
        ${src_dir}/view/home/MenuItem.hpp
        ${src_dir}/view/home/MenuView.hpp
        ${src_dir}/view/tools/Tools.hpp
        ${src_dir}/view/tools/StringTools.hpp
        ${src_dir}/view/tools/ViewFocusOperator.hpp
        ${src_dir}/view/widget/ListView.hpp
        ${src_dir}/view/widget/TextEditView.hpp
        ${src_dir}/view/widget/TextView.hpp
        ${src_dir}/view/widget/ButtonView.hpp
        ${src_dir}/view/widget/TagPickerView.hpp
        ${src_dir}/view/dialog/MessageDialog.hpp
        ${src_dir}/view/dialog/ConfirmDialog.hpp
        ${src_dir}/model/Memo.hpp
        ${src_dir}/model/Tag.hpp
        ${src_dir}/model/ModelDefs.hpp
)

# OpenSSL
if (APPLE)
    # This is a bug in CMake that causes it to prefer the system version over
    # the one in the specified ROOT folder.
    set(OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR} /usr/local/opt/openssl)
    set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib CACHE FILEPATH "" FORCE)
    set(OPENSSL_SSL_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libssl.dylib CACHE FILEPATH "" FORCE)
endif()
find_package(OpenSSL REQUIRED)

find_package(Threads REQUIRED)

# Link ncurses libs
set(CURSES_NEED_NCURSES TRUE)
set(curses_lib_dir)
set(curses_include_dir)
if (CURSES_DIR)
    message("Searching for ncurses in: " ${CURSES_DIR})
    find_library(NCURSES_LIB ncurses PATHS ${CURSES_DIR}/lib REQUIRED)
    find_library(MENU_LIB menu PATHS ${CURSES_DIR}/lib REQUIRED)
    set(curses_lib_dir ${CURSES_DIR}/lib)
    set(curses_include_dir ${CURSES_DIR}/include)
else()
    find_package(Curses REQUIRED)
    find_library(menu_lib menu REQUIRED)
    get_filename_component(curses_lib_dir ${menu_lib} DIRECTORY)
    get_filename_component(curses_dir ${curses_lib_dir} DIRECTORY)
    set(curses_include_dir ${curses_lib_dir}/include)
endif()

message("Linked ncurses library directory: " ${curses_lib_dir})
message("Ncurses include directory: " ${curses_include_dir})

target_include_directories(${exe_lib}
    PRIVATE
        ${src_dir}
    PUBLIC
        ${curses_include_dir}
        ${memo_proto_lib_include_dir})

target_link_directories(${exe_lib} PUBLIC ${curses_lib_dir})
target_link_libraries(${exe_lib}
    PUBLIC
        ncurses
        menu
        ${memo_proto_lib})

target_link_libraries(${exe} PRIVATE ${exe_lib})