use rand::prelude::SliceRandom;

use crate::individuo::Individuo;
use crate::mapa::Mapa;
use crate::solucao::Solucao;

/// Armazena uma conjunto de soluções,
/// onde cada solução é um indivíduo da população
///
/// Possui metódos para ser população do algoritmo evolutivo
pub struct Populacao {
    pub pop: Vec<Individuo>,
}

impl Populacao {
    pub fn criar(mapa: &Mapa, n: usize) -> Populacao {
        Populacao {
            pop: (0..n).map(|_| Individuo::random(mapa)).collect(),
        }
    }

    /// Calcula o fitness do indivíduo em relação à população
    fn individuo_fitness(&mut self, mapa: &Mapa, individuo: &mut Individuo) -> i32 {
        self.pop
            .iter_mut()
            .fold(0, |acc, ind| acc + individuo.compete(mapa, ind))
    }

    /// Calcula o fitness de cada indivíduo da população
    /// e ordena a população pelo fitness
    pub fn fitness(&mut self, mapa: &Mapa) {
        // for individuo in &mut self.pop {
        //     individuo.fitness = self.individuo_fitness(mapa, individuo);
        // }

        // self não pode fazer mais de um borrow_mut()
        for i in 0..self.pop.len() {
            let mut individuo = self.pop[i].clone();
            individuo.fitness = self.individuo_fitness(mapa, &mut individuo);
            self.pop[i] = individuo;
        }

        self.pop.sort_unstable_by(|a, b| b.fitness.cmp(&a.fitness));
    }

    /// Cria uma nova população usando o metódo elitismo
    ///
    /// Considera que o primeiro indivíduo é o melhor de todos
    pub fn _elitismo(&mut self, mapa: &Mapa) {
        let melhor = &self.pop[0];
        self.pop = self
            .pop
            .iter()
            .map(|i| Individuo::crossover(mapa, melhor, i))
            .collect();
    }

    /// Cria uma nova população
    /// usando o metódo torneio de dois baseado no fitness dos indivíduos
    ///
    /// Considera que o primeiro indivíduo é o melhor de todos
    pub fn _torneio(&mut self, mapa: &Mapa) {
        let mut random = rand::thread_rng();
        self.pop = self
            .pop
            .iter()
            .map(|_| {
                let pai = {
                    let a = self.pop.choose(&mut random).unwrap();
                    let b = self.pop.choose(&mut random).unwrap();
                    if a.fitness > b.fitness {
                        a
                    } else {
                        b
                    }
                };
                let mae = {
                    let a = self.pop.choose(&mut random).unwrap();
                    let b = self.pop.choose(&mut random).unwrap();
                    if a.fitness > b.fitness {
                        a
                    } else {
                        b
                    }
                };

                Individuo::crossover(mapa, pai, mae)
            })
            .collect();
    }

    /// Aplica mutação na população
    ///
    /// Cada gene tem o valor do parâmetro mutação de chance de sofrer mutação
    ///
    /// É garantido que pelo menos um gene de cada indivíduo sofrerá mutação
    pub fn _mutacao(&mut self, mutacao: f64) {
        self.pop.iter_mut().for_each(|i| i.mutacao(mutacao));
    }

    /// Gera uma nova por predação por síntese
    /// (indivíduo com os genes mais comuns na população)
    /// e troca pelo pior de todos exceto os `n` pior que é da predação radômica
    pub fn _predacao_sintese(&mut self, mapa: &Mapa, n: usize) {
        let tam = mapa.tam_cromossomo;

        let cromossomo = (0..tam)
            .map(|i| {
                let freq = self.pop.iter().fold([0; 9], |mut freq, ind| {
                    freq[ind.solucao.cromossomo[i].unwrap() as usize] += 1;
                    freq
                });

                let index = freq
                    .iter()
                    .enumerate()
                    .max_by_key(|(_, &val)| val)
                    .unwrap()
                    .0;

                Some(index as u8)
            })
            .collect();

        let mut sintese = Individuo {
            solucao: Solucao { cromossomo },
            fitness: 0,
        };
        sintese.fitness = self.individuo_fitness(mapa, &mut sintese);

        *self.pop.iter_mut().nth_back(n + 1).unwrap() = sintese;
    }

    /// Gera n novas soluções aleatórias trocando as piores soluções
    pub fn _predacao_randomica(&mut self, mapa: &Mapa, n: usize) {
        // self.pop.iter_mut().rev().take(n).for_each(|i| {
        //     let mut ind = Individuo::random(tam);
        //     ind.fitness = self.individuo_fitness(mapa, &mut ind);
        //     *i = ind;
        // });

        // self não pode fazer mais de um borrow_mut()
        for i in n..self.pop.len() {
            let mut ind = Individuo::random(mapa);
            ind.fitness = self.individuo_fitness(mapa, &mut ind);
            self.pop[i] = ind;
        }
    }
}
