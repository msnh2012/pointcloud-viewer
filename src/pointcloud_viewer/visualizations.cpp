#include <pointcloud_viewer/visualizations.hpp>
#include <core_library/color_palette.hpp>

#include <stdio.h>

Visualization::Visualization()
  : world_axis(DebugMesh::axis()),
    world_grid(DebugMesh::grid(5, 1.f, color_palette::grey[1])),
    turntable_origin(DebugMesh::turntable_point(glm::vec3(0))),
    trackball(DebugMesh::trackball(glm::vec3(0))),
    camera_path(DebugMesh::path(0, [](int)->frame_t{Q_UNREACHABLE();}, -1))
{
}

void Visualization::render()
{
  if(!settings.enable_any_visualizations)
    return;

  debug_mesh_renderer.begin();
  if(settings.enable_grid)
    debug_mesh_renderer.render(world_grid);
  if(settings.enable_axis)
    debug_mesh_renderer.render(world_axis);
  if(settings.enable_turntable_center)
    debug_mesh_renderer.render(turntable_origin);
  if(settings.enable_trackball)
    debug_mesh_renderer.render(trackball);
  if(settings.enable_camera_path)
    debug_mesh_renderer.render(camera_path);
  if(settings.enable_picked_cone)
    debug_mesh_renderer.render(picked_cone);
  if(settings.enable_selected_point)
    debug_mesh_renderer.render(selected_point);
  if(settings.enable_kdtree_as_aabb)
  {
    debug_mesh_renderer.render(kdtree_current_point);
    debug_mesh_renderer.render(kdtree_normal_aabb);
    debug_mesh_renderer.render(kdtree_highlight_aabb);
  }
  debug_mesh_renderer.end();
}

void Visualization::set_turntable_origin(glm::vec3 origin)
{
  turntable_origin = DebugMesh::turntable_point(origin);
}

void Visualization::set_path(const QVector<keypoint_t>& keypoints, int selected_point)
{
  camera_path = DebugMesh::path(keypoints.length(), [&keypoints](int i){return keypoints[i].frame;}, selected_point);
}

void Visualization::set_kdtree_as_aabb(aabb_t highlighted_aabb, glm::vec3 separator_point, aabb_t other_aabb)
{
  glm::vec3 normal_color = glm::vec3(1,0.5,0);
  glm::vec3 highlight_color = glm::vec3(1,1,0.2);

  kdtree_current_point = DebugMesh::turntable_point(separator_point, 0.25f, normal_color);
  kdtree_normal_aabb = DebugMesh::aabb(other_aabb, normal_color);
  kdtree_highlight_aabb = DebugMesh::aabb(highlighted_aabb, highlight_color);
}

void Visualization::set_picked_cone(cone_t picked_cone)
{
  this->picked_cone = DebugMesh::cone(picked_cone);
}

void Visualization::deselect_picked_point()
{
  this->selected_point = DebugMesh();
}

void Visualization::select_picked_point(glm::vec3 coordinate, glm::u8vec3 color, float radius)
{
  this->selected_point = DebugMesh::highlighted_point(coordinate, glm::vec3(color) / 255.f, radius);
}

void Visualization::set_trackball(glm::vec3 center, float radius)
{
  trackball = DebugMesh::trackball(center, radius);
}

Visualization::settings_t Visualization::settings_t::enable_all()
{
  settings_t settings;

  memset(&settings, 0xff, sizeof(settings));

  return settings;
}

Visualization::settings_t Visualization::settings_t::default_settings()
{
  settings_t settings = enable_all();

  settings.enable_turntable_center = false;
  settings.enable_kdtree_as_aabb = false;
  settings.enable_picked_cone = false;
  settings.enable_trackball = false;

  return settings;
}

Visualization::settings_t Visualization::settings_t::disable_all()
{
  settings_t settings;

  memset(&settings, 0, sizeof(settings));

  return settings;
}
