use rand::prelude::SliceRandom;

use crate::individuo::Individuo;
use crate::mapa::Mapa;
use crate::solucao::Solucao;

pub struct Populacao {
    pub pop: Vec<Individuo>,
}

impl Populacao {
    pub fn criar(tam: usize, n: usize) -> Populacao {
        Populacao {
            pop: (0..n).map(|_| Individuo::random(tam)).collect(),
        }
    }

    fn individuo_fitness(&mut self, mapa: &Mapa, individuo: &mut Individuo) -> i32 {
        self.pop
            .iter_mut()
            .fold(0, |acc, ind| acc + individuo.compete(mapa, ind))
    }

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

        self.pop.sort_unstable_by_key(|ind| ind.fitness);
    }

    pub fn elitismo(&mut self, tam: usize, populacao: Populacao) {
        let melhor = &self.pop[0];
        self.pop = self
            .pop
            .iter()
            .map(|i| Individuo::crossover(tam, melhor, i))
            .collect();
    }

    pub fn torneio(&mut self, tam: usize) {
        let mut random = rand::thread_rng();
        self.pop = (0..tam)
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

                Individuo::crossover(tam, pai, mae)
            })
            .collect();
    }

    pub fn mutacao(&mut self, mutacao: f64) {
        self.pop.iter_mut().for_each(|i| i.mutacao(mutacao));
    }

    pub fn predacao_sintese(&mut self, mapa: &Mapa, n: usize) {
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

    pub fn predacao_randomica(&mut self) {
        todo!() // TODO
    }
}
