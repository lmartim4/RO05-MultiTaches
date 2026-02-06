# RO05

[[TP Link] Professor's website](https://irfu.cea.fr/Pisp/shebli.anvar/prog-mt-rtos/md__h_1_2_activites_2_enseignement_2_cours-2025-_web_2ensta_2_e_n_s_t_a-_instructions.html#autotoc_md1)


## Auto exec in Raspberry

```bash
cd ~
git clone git@github.com:lmartim4/RO05-MultiTaches.git
echo -e '\nalias rpi-deploy="~/RO05-MultiTaches/rpi-deploy"\ncd ~/RO05-MultiTaches' >> ~/.bashrc
source ~/.bashrc
```

Test it

```bash
cd TP1
rpi-deploy main.cpp
```