FROM thecodingmachine/php:7.2-v2-apache

#ENV http_proxy http://172.17.0.1:5865
#ENV https_proxy http://172.17.0.1:5865

#COPY apt.conf /etc/apt/apt.conf

RUN sudo apt-get update
RUN sudo apt-get -y install imagemagick wget net-tools curl

RUN echo -h

RUN echo "Listen 8080\nListen 80" | sudo tee /etc/apache2/ports.conf

RUN cat /etc/apache2/ports.conf

RUN sudo sh -c "echo '\nexport APACHE_DOCUMENT_ROOT=\n' >> /etc/apache2/envvars"
RUN sudo sed -i 's#AllowOverride [Nn]one#AllowOverride All#' /etc/apache2/apache2.conf
RUN sudo sed -i 's#AllowOverride [Nn]one#AllowOverride All#' /etc/apache2/sites-enabled/000-default.conf

EXPOSE 80
