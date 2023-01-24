using Printf

nhilos = Int(Threads.nthreads())
num_args = length(ARGS)

L = length(ARGS) > 0 ? parse(Float64, ARGS[1]) : 1 # longitud del dominio
T = length(ARGS) > 1 ? parse(Float64, ARGS[2]) : 1 # tiempo total de simulación
alpha = length(ARGS) > 2 ? parse(Float64, ARGS[3]) : 0.01 # coeficiente de difusión
dx = 0.1 # tamaño de paso en x
dt = 0.1 # tamaño de paso en t
nx = Int(L/dx + 1) # número de puntos en x
nt = Int(T/dt + 1) # número de puntos en t

u = Array{Float64}(undef, nx, nt) # matriz para almacenar la solución

# Condiciones iniciales
for i in 1:nx
    u[i,1] = 10 # se establece un valor constante para todos los puntos iniciales
end

# Condiciones de frontera
for j in 1:nt
    u[1,j] = 0
    u[nx,j] = 0
end

channels = [Channel(1) for i in 1:nhilos]

Threads.@threads for i in 1:nhilos
    if i > 1
        take!(channels[i-1]) # Espera hasta recibir un mensaje del hilo anterior antes de comenzar su trabajo
    end
    for j in 1:nt-1
        for i in 2:nx-1
            u[i,j+1] = u[i,j] + alpha * dt / dx^2 * (u[i+1,j] - 2 * u[i,j] + u[i-1,j])
        end
    end
    put!(channels[i], true) # Envía un mensaje al hilo siguiente para indicar que ha terminado su trabajo
end


# Imprimir resultado en consola
@printf("%-10s %-15s %-10s\n", "Posición", "Temperatura", "Tiempo")
for i in 1:nx-1
    for j in 1:nt-1
        @printf("%-10.5f %-15.5f %-10.5f\n", i*dx, u[i,j], j*dt)
    end
end
