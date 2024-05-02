# Planet-Donut
Contains the Planet Donut project

For more details of the strategy and code, please refer to "rendu_3_de_prog.pdf"

## Project description

![Initial planet (ressources and bases)](/img/c1.jpg){:height="10%" width="10%"}
![During execution](/img/c2.jpg){:height="30%" width="30%"}

Collaborated with a classmate on the Planet Donut project, focusing on the development and implementation of diverse algorithms for robot control in a simulated environment. These algorithms were designed for tasks including exploration, resource extraction, and communication network establishment.

For connectivity, utilized a depth-first search algorithm starting from a central communication robot, extending to its neighbors and their subsequent neighbors, ensuring connectivity within a vector of base robots.

Exploration involved dividing space into equal parts relative to the base center, with prospecting robots moving diagonally upwards to cover the minimum deposit radius on the ordinate axis. Up to 10 robots were deployed to ensure comprehensive planet exploration.

Drilling robots were deployed to newly discovered resource deposits, ensuring no existing drilling robots were present at the target site.

Transport robots headed to each deposit with a drilling robot, prioritizing deposits with the most resources if there were more deposits than transport robots. They collected resources and returned them to the base for processing, awaiting further instructions once all deposits were depleted.

Communication strategy involved deploying a network of 49 communication robots to cover the entire planet. Arranged in 7 lines of 7 robots, approximately 286 km apart, these stationary robots minimized maintenance needs due to their limited movement range compared to maximum travel distance.

The approach prioritized rapid information and resource collection while minimizing unnecessary robot production, ensuring efficient exploration, resource extraction, and base autonomy.
