@echo off 
IF "%1"=="/help" (
    ECHO Execute the batch file to sign the WCD driver using TestCertificate. 
    ECHO Add the flag /cert to create the test certificate.
    GOTO end
)

IF "%1"=="/cert" (
    ECHO Creating test certificate.
    REM Creates test certificate to sign the driver
    CALL "makecert /r /pe /ss PrivateCertStore /n CN=TestCertificate.com(Test) TestCertificate.cer"
) ELSE (
    ECHO Only signing operation.
)

REM Signs the driver with test certificate
CALL signtool sign /v /a /s PrivateCertStore /n TestCertificate.com(Test) /fd SHA256 /t http://timestamp.digicert.com build\WCD\Debug\WCD.sys

:end
