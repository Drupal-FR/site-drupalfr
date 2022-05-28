ARG DOCKER_SERVICE_WEB_IMAGE

FROM ${DOCKER_SERVICE_WEB_IMAGE}
ENV USER docker
ENV GROUP docker
ENV APACHE_RUN_USER docker
ENV APACHE_RUN_GROUP docker
ENV FIXUID_VERSION 0.5.1

RUN groupadd -g 1000 ${GROUP} && \
    useradd -u 1000 -g ${GROUP} -d /home/${USER} -s /bin/sh ${USER} && \
    mkdir /home/${USER} && \
    chown ${USER}:${GROUP} /home/${USER}

RUN echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker && \
    chmod 440 /etc/sudoers.d/docker

COPY ./bin/entrypoint.sh /bin/

# Install fixuid
RUN curl -SsL https://github.com/boxboat/fixuid/releases/download/v${FIXUID_VERSION}/fixuid-${FIXUID_VERSION}-linux-amd64.tar.gz | tar -C /usr/local/bin -xzf - && \
		chown root:root /usr/local/bin/fixuid && \
		chmod 4755 /usr/local/bin/fixuid && \
		mkdir -p /etc/fixuid && \
		printf "user: ${USER}\ngroup: ${GROUP}\npaths:\n  - /var/log\n  - /home/docker\n" > /etc/fixuid/config.yml

RUN apt-get update && \
    apt-get install libcap2-bin -y && \
    rm -rf /var/cache/apt/archives/ && \
    setcap cap_net_bind_service=+ep /usr/local/bin/apache2-foreground && \
    setcap cap_net_bind_service=+ep /usr/sbin/apache2

RUN chmod 666 /var/log/drupal_cron.log

USER docker

ENTRYPOINT ["/bin/entrypoint.sh"]
CMD ["/usr/local/bin/apache2-foreground"]
