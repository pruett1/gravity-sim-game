# gravity-sim-game
Recreating this planet gravity simulation game that I messed around with in highschool

## Physics Background/Comments
The physics of this is relatively simple, as I am just going to be using classical physics (no general relativity)

So, $\vec{F_g} = m_0 \vec{a}= \sum_{n=1}^{N} G \frac{m_0m_n}{r^2} \hat{r}$, where $\hat{r} = \frac{(x_n-x_0, y_n-y_0)}{r}$

Therefore $\vec{a} = \sum_{n=1}^{N} G \frac{m_n}{\vec{r}^2}$

This can then be split up so that $a_x \hat{x} = \sum_{n=1}^{N} G \frac{m_n*(x_n-x_0)}{r^3}\hat{x}$ and $a_y \hat{y} = \sum_{n=1}^{N} G \frac{m_n*(y_n-y_0)}{r^3}\hat{y}$

For the computation of this, I am using C++ but will expose it to Python using pybind11

The evolution of the masses will be done using RK4, the stellar objects will only be able to traverse through the x-y plane

For RK4, the state is position and the velocity, the derivative of the state then is $\frac{d\vec{l}}{dt} = \vec{v}$ and $\frac{d\vec{v}}{dt} = \vec{a}$
- $k1_p = v_t$, $k1_v = \vec{a}(p_t)$, $p_1 = p_0 + \frac{1}{2} k1_p dt$, $v_1 = v_0 + \frac{1}{2} k1_v dt$
- $k2_p = v_1$, $k2_v = \vec{a}(p_1)$, $p_2 = p_1 + \frac{1}{2} k2_p dt$, $v_2 = v_1 + \frac{1}{2} k2_v dt$
- $k3_p = v_2$, $k3_v = \vec{a}(p_2)$, $p_3 = p_2 + k3_p dt$, $v_3 = v_2 + k3_v  dt$
- $k4_p = v_3$, $k4_v = \vec{a}(p_3)$

Then
- $p_{t+1} = p_t + \frac{1}{6}(k1_p + 2 k2_p + 2 k3_p + k4)$
- $v_{t+1} = v_t + \frac{1}{6}(k1_v + 2 k2_v + 2 k3_v + k4)$


## Visualization/Interaction
I will be using pygame to visualize and allow the interaction with the environment

With the pygame interface should be able to
- Place masses of differing densities
- Initialize those masses with different starting velocities
- Observe the interaction of these masses
- Reset the environment
- Stop and start time (so you can set up a whole environment and see the evolution)