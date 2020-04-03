# CLion remote docker environment (How to build docker container, run and stop it)
#
# Build and run:
#   docker build -t granite_ssh -f Dockerfile .
#   docker run -d --cap-add sys_ptrace -p127.0.0.1:2222:22 --name granite_ssh granite_ssh
#   ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:2222"
#
# stop:
#   docker stop granite_ssh
#
# ssh credentials (test user):
#   user@password
FROM stmario/sheep_base AS granite_ssh

RUN apt-get update \
  && apt-get install -y ssh \
      build-essential \
      gdb \
      clang \
      rsync \
      python \
  && apt-get clean

RUN ( \
    echo 'LogLevel DEBUG2'; \
    echo 'PermitRootLogin yes'; \
    echo 'PasswordAuthentication yes'; \
    echo 'Subsystem sftp /usr/lib/openssh/sftp-server'; \
  ) > /etc/ssh/sshd_config_test_clion \
  && mkdir /run/sshd

RUN useradd -m user \
  && yes password | passwd user

CMD ["/usr/sbin/sshd", "-D", "-e", "-f", "/etc/ssh/sshd_config_test_clion"]