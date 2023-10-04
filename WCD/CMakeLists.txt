cmake_minimum_required(VERSION 3.0)

project(WCD)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(WDK REQUIRED)

#target_include_directories(WCD INTERFACE .)

wdk_add_driver(WCD KMDF 1.15
        wcd.cpp
	)

if (CREATE_CERT)
    set( CERTIFICATE_ARGS -r -pe -ss PrivateCertStore -n CN=TestCertificate.com TestCertificate.cer )
    add_custom_command(TARGET WCD POST_BUILD
        COMMAND MakeCert.exe ARGS ${CERTIFICATE_ARGS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BIN_DIR}
        COMMENT "Creating test certificate."
)
endif()

if (SIGN_DRIVER)
    set(SIGN_ARGS sign /v /a /s PrivateCertStore /n TestCertificate.com /fd SHA256 /t http://timestamp.digicert.com $<CONFIG>//WCD.sys )
    add_custom_command(TARGET WCD POST_BUILD
        COMMAND signtool.exe ARGS ${SIGN_ARGS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BIN_DIR}
        COMMENT "Signing driver with test certificate."
)
endif()
